// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControl.h"

#include <chrono>

#include "GameFramework/SpringArmComponent.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <InputTriggers.h>
#include <iomanip>
#include <sstream>
#include<Camera/CameraComponent.h>
#include "DrawDebugHelpers.h"
#include "DrawingUI.h"
#include "EngineUtils.h"
#include "ImageUtils.h"
#include "ArtHeal/LHJ/ApiActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/FileHelper.h"
#include "UObject/ConstructorHelpers.h"
#include "YJ/PaintTarget.h"
#include "PlayerAnim.h"
#include "AnalyzeUI.h"
#include "Engine/TextureRenderTarget2D.h"


// Sets default values
APlayerControl::APlayerControl()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. ���̷�Ż �޽� �����͸� �ҷ����� �ʹ�. TEXT ������ ������ ���
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	//���� ������ �ε��ϴµ� �����Ѵٸ�
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. Mesh ������Ʈ�� ��ġ�� ȸ�� ���� �����ϰ� �ʹ�.
		//GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, -90));
		//3. �޽��� �ݸ��� ������ NoCollision���� �����Ѵ�.
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//3. TPS ī�޶� ���̰� �ʹ�.
	//3-1. SpringArm ������Ʈ ���̱�
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 80));
	springArmComp->TargetArmLength = 400;
	//���Ʒ��� �����Ӱ� �� �� �ֵ��� ����


	springArmComp->bUsePawnControlRotation = true;
	//3-2. Camera ������Ʈ ���̱�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;
	//bUseControllerRotationYaw = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
}

// Called when the game starts or when spawned
void APlayerControl::BeginPlay()
{
	Super::BeginPlay();

	pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(imc_Player, 0);
		}
	}

	anim = Cast<UPlayerAnim>(GetMesh()->GetAnimInstance());

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(FName("api")))
		{
			ApiActor = Cast<AApiActor>(Actor);
		}
	}
	DrawingUI = CreateWidget<UDrawingUI>(GetWorld(), DrawingUIFactory);
	if (DrawingUI)
	{
		DrawingUI->AddToViewport();
		DrawingUI->SetVisibility(ESlateVisibility::Hidden);
		DrawingUI->SetArtTitle();
	}

	AnalyzeUI = CreateWidget<UAnalyzeUI>(GetWorld(), AnalyzeUIFactory);
	if (AnalyzeUI)
	{
		AnalyzeUI->AddToViewport();
		AnalyzeUI->SetVisibility(ESlateVisibility::Hidden);
	}

	Paintable = Cast<APaintTarget>(UGameplayStatics::GetActorOfClass(this, APaintTarget::StaticClass()));
}

// Called every frame
void APlayerControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove(DeltaTime); //�̵� ó��
}

// Called to bind functionality to input
void APlayerControl::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &APlayerControl::Turn);
		PlayerInput->BindAction(ia_LookUp, ETriggerEvent::Triggered, this, &APlayerControl::LookUp);

		PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &APlayerControl::Move);
		PlayerInput->BindAction(ia_Paint, ETriggerEvent::Triggered, this, &APlayerControl::Paint);
	}
}

void APlayerControl::Turn(const FInputActionValue& inputValue)
{
	if (bPainting)
	{
		return;
	}
	float value = inputValue.Get<float>();
	float turnRate = 0.4f;
	AddControllerYawInput(value * turnRate);
}

void APlayerControl::LookUp(const FInputActionValue& inputValue)
{
	if (bPainting)
	{
		return;
	}
	float value = inputValue.Get<float>();
	float lookUpRate = 1.0f;
	AddControllerPitchInput(value * lookUpRate);
}

void APlayerControl::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	direction.X = value.X;
	direction.Y = value.Y;
	direction.Normalize();
}


//�÷��̾� �̵�ó�� �Լ�
void APlayerControl::PlayerMove(float DeltaTime)
{
	/*FVector normalizedDirection = FTransform(GetControlRotation()).TransformVector(direction);
	normalizedDirection.Normalize();

	FVector P0 = GetActorLocation();
	FVector vt = normalizedDirection * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);
	direction = FVector::ZeroVector;*/

	direction = FTransform(GetControlRotation()).TransformVector(direction);

	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void APlayerControl::Paint()
{
	FHitResult CursorHit;
	pc->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if (CursorHit.GetActor())
	{
		AActor* hitActor = CursorHit.GetActor();
		Paintable = Cast<APaintTarget>(hitActor);
		if (Paintable)
		{
			FVector_NetQuantize ImpactPoint = CursorHit.ImpactPoint;
			MyVector = FVector(ImpactPoint);
			FString VectorString = MyVector.ToString();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, VectorString);
		}
	}


	FVector StartLocation = GetActorLocation();

	MyVector.X += 3.f;

	FVector EndLocation = MyVector;

	FString VectorString = EndLocation.ToString();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, VectorString);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnFaceIndex = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Emerald, false, 1, 5, 2.f);

	if (HitResult.bBlockingHit)
	{
		AActor* hitActor = HitResult.GetActor();
		Paintable = Cast<APaintTarget>(hitActor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "HitResult_Success");
		if (Paintable)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Paintable_Success");

			FVector2D uv;

			bool result = UGameplayStatics::FindCollisionUV(HitResult, 0, uv);

			FString VectorString_ = uv.ToString();

			// On-Screen Debug 메시지로 출력

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, VectorString_);

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, result ? TEXT("bIsTrue: true") : TEXT("bIsTrue: false"));

			Paintable->Painted(brushNum, uv, 70);
		}
	}
}

void APlayerControl::SaveTexture()
{
	//�����׸� �׸��� TextureRenderTarget ���� �޾Ƽ� �����ϰ� �ʹ�
	UTextureRenderTarget2D* TexRT_ = Paintable->CRT_PaintMask;

	FImage Image;
	if (!FImageUtils::GetRenderTargetImage(TexRT_, Image))
	{
		return;
	}

	TArray64<uint8> CompressedData;
	if (!FImageUtils::CompressImage(CompressedData,TEXT("JPEG"), Image))
	{
		return;
	}
	FString MainFileName = "TEST";
	FString TimeStr = GetFormattedDateTime();
	FString FileName = FString::Printf(TEXT("/%s_%s.jpeg"), *MainFileName, *TimeStr);
	FString imagePath = FPaths::ProjectPersistentDownloadDir() + FileName;

	// ����Լ� FPaths::ProjectPersistentDownloadDir() :
	// ==> ��� : ../../../../../../../../Users/(����ڸ�)/Desktop/General/Project/Prototype/(�𸮾� ������Ʈ��)/PersistentDownload
	// ������  C:\UnrealProjects\HttpProject\Saved\PersistentDownloadDir �� ����.

	FFileHelper::SaveArrayToFile(CompressedData, *imagePath);

	ApiActor->ReqPostText(imagePath, CompressedData);
}

void APlayerControl::SetBrushNum(int32 num)
{
	brushNum = num;
}

void APlayerControl::SetDrawingUIVisible(bool value)
{
	//auto pc = Cast<APlayerController>(Controller);
	if (value)
	{
		pc->SetShowMouseCursor(true);
		pc->SetInputMode(FInputModeGameAndUI());
		DrawingUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		//pc->SetShowMouseCursor(false);
		pc->SetInputMode(FInputModeGameAndUI());
		DrawingUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayerControl::ShowMouseCursor(bool value)
{
	pc->SetShowMouseCursor(value);
}


FString APlayerControl::GetFormattedDateTime()
{
	// 현재 시간 가져오기
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);

	// 시간 정보를 `std::tm` 구조체로 변환
	std::tm tm_now;
#ifdef _WIN32
	localtime_s(&tm_now, &now_time);
#else
		tm_now = *std::localtime(&now_time);
#endif

	// FString으로 포맷 설정
	FString formattedDateTime = FString::Printf(TEXT("%04d%02d%02d_%02d%02d%02d"),
	                                            tm_now.tm_year + 1900, // 년도는 1900을 더해줘야 함
	                                            tm_now.tm_mon + 1, // 월은 0부터 시작하므로 1을 더함
	                                            tm_now.tm_mday,
	                                            tm_now.tm_hour,
	                                            tm_now.tm_min,
	                                            tm_now.tm_sec);

	// 최종 FString 반환
	return formattedDateTime;
}
