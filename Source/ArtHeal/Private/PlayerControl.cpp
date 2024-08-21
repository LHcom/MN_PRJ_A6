// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControl.h"
#include "GameFramework/SpringArmComponent.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <InputTriggers.h>
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

// Sets default values
APlayerControl::APlayerControl()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. ���̷�Ż �޽� �����͸� �ҷ����� �ʹ�. TEXT ������ ������ ���
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	//���� ������ �ε��ϴµ� �����Ѵٸ�
	if (TempMesh.Succeeded()) {
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
	
	auto pc = Cast<APlayerController>(Controller);
	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem) {
			subsystem->AddMappingContext(imc_Player, 0);
		}
	}

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(FName("api")))
		{
			ApiActor = Cast<AApiActor>(Actor);
		}
	}
	DrawingUI=CreateWidget<UDrawingUI>(GetWorld(),DrawingUIFactory);
	if (DrawingUI)
	{
		DrawingUI->AddToViewport();
		DrawingUI->SetVisibility(ESlateVisibility::Hidden);
	}

	Paintable =Cast<APaintTarget>(UGameplayStatics::GetActorOfClass(this,APaintTarget::StaticClass()));
	
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
	if (PlayerInput) {
		PlayerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &APlayerControl::Turn);
		PlayerInput->BindAction(ia_LookUp, ETriggerEvent::Triggered, this, &APlayerControl::LookUp);

		PlayerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &APlayerControl::Move);
		PlayerInput->BindAction(ia_Paint, ETriggerEvent::Triggered, this, &APlayerControl::Paint);
	}
}

void APlayerControl::Turn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	float turnRate = 0.4f;
	AddControllerYawInput(value * turnRate);
}

void APlayerControl::LookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	float lookUpRate = 1.0f;
	AddControllerPitchInput(value * lookUpRate);
}

void APlayerControl::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	//���� �Է� �̺�Ʈ ó��
	direction.X = value.X;
	// �¿� �Է� �̺�Ʈ ó��
	direction.Y = value.Y;
	// ���� ���� ����ȭ, �밢���ϸ� �ӵ� �������� �ʰ� �ϱ�
	direction.Normalize();
}

//�÷��̾� �̵�ó�� �Լ�
void APlayerControl::PlayerMove(float DeltaTime) {
	FVector normalizedDirection = FTransform(GetControlRotation()).TransformVector(direction);
	normalizedDirection.Normalize();

	FVector P0 = GetActorLocation();
	FVector vt = normalizedDirection * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);
	direction = FVector::ZeroVector;
}

void APlayerControl::Paint()
{
	FRotator ViewRotation;
	FVector Viewlocation;
	GetController()->GetPlayerViewPoint(Viewlocation, ViewRotation);

	//FVector StartLocation = GetActorLocation();
	FVector StartLocation = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector()*10;

	//FVector EndLocation = Viewlocation + ViewRotation.Vector() * fireTraceDistance;
	FVector EndLocation = StartLocation + tpsCamComp->GetForwardVector()* fireTraceDistance;
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	//CollisionParams. ???? ???. 
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnFaceIndex = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility,CollisionParams);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Emerald, false, 1, 5, 2.f);
	if (HitResult.bBlockingHit) //��Ʈ����� ������ 
	{
		AActor* hitActor = HitResult.GetActor();
		Paintable = Cast<APaintTarget>(hitActor);
		if(Paintable)
		{
			FVector2D uv;
			UGameplayStatics::FindCollisionUV(HitResult, 0, uv);
			
			Paintable->Painted(brushNum,uv,50);
		}
	}
}

void APlayerControl::SaveTexture()
{
	//�����׸� �׸��� TextureRenderTarget ���� �޾Ƽ� �����ϰ� �ʹ�
	UTextureRenderTarget2D* TexRT_ = Paintable->CRT_PaintMask;
	
	FImage Image;
	if ( ! FImageUtils::GetRenderTargetImage(TexRT_,Image) )
	{
		return;
	}

	TArray64<uint8> CompressedData;
	if ( ! FImageUtils::CompressImage(CompressedData,TEXT("PNG"),Image) )
	{
		return ;
	}

	FString imagePath = FPaths::ProjectPersistentDownloadDir()+"/Painted.jpg";

	// ����Լ� FPaths::ProjectPersistentDownloadDir() :
	// ==> ��� : ../../../../../../../../Users/(����ڸ�)/Desktop/General/Project/Prototype/(�𸮾� ������Ʈ��)/PersistentDownload
	// ������  C:\UnrealProjects\HttpProject\Saved\PersistentDownloadDir �� ����.

	FFileHelper::SaveArrayToFile(CompressedData,*imagePath);
	
	//ApiActor->ReqPostText("Painted.jpg",CompressedData);
}

void APlayerControl::SetBrushNum(int32 num)
{
	brushNum=num;
}

void APlayerControl::SetDrawingUIVisible(bool value)
{
	auto pc = Cast<APlayerController>(Controller);
	if(value)
	{
		pc->SetShowMouseCursor(true);
		pc->SetInputMode(FInputModeGameAndUI());
		DrawingUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		pc->SetShowMouseCursor(false);
		pc->SetInputMode(FInputModeGameOnly());
		DrawingUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

