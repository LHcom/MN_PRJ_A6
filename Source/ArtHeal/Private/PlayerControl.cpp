// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControl.h"
#include "GameFramework/SpringArmComponent.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <InputTriggers.h>
#include<Camera/CameraComponent.h>

#include "DrawDebugHelpers.h"
#include "ImageUtils.h"
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

	// 1. 스켈레탈 메시 데이터를 불러오고 싶다. TEXT 다음은 에셋의 경로
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	//만약 에셋을 로드하는데 성공한다면
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. Mesh 컴포넌트의 위치와 회전 값을 설정하고 싶다.
		//GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, -90));
		//3. 메쉬의 콜리전 설정은 NoCollision으로 변경한다.
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	//3. TPS 카메라를 붙이고 싶다.
	//3-1. SpringArm 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 80));
	springArmComp->TargetArmLength = 400;
	//위아래로 자유롭게 볼 수 있도록 설정


	springArmComp->bUsePawnControlRotation = true;
	//3-2. Camera 컴포넌트 붙이기
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
	
}

// Called every frame
void APlayerControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PlayerMove(DeltaTime); //이동 처리

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
	AddControllerYawInput(value);
}

void APlayerControl::LookUp(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value);
}

void APlayerControl::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	//상하 입력 이벤트 처리
	direction.X = value.X;
	// 좌우 입력 이벤트 처리
	direction.Y = value.Y;
	// 방향 벡터 정규화, 대각선하면 속도 빨라지지 않게 하기
	direction.Normalize();
}

//플레이어 이동처리 함수
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
	if (HitResult.bBlockingHit) //히트대상이 있을때 
	{
		AActor* hitActor = HitResult.GetActor();
		Paintable = Cast<APaintTarget>(hitActor);
		if(Paintable)
		{
			FVector2D uv;
			UGameplayStatics::FindCollisionUV(HitResult, 0, uv);

			Paintable->Painted(0,uv,50);
		}
	}
}

void APlayerControl::SaveTexture(UTextureRenderTarget2D* TexRT)
{
	//내가그린 그림을 TextureRenderTarget 으로 받아서 저장하고 싶다
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

	// 경로함수 FPaths::ProjectPersistentDownloadDir() :
	// ==> 결과 : ../../../../../../../../Users/(사용자명)/Desktop/General/Project/Prototype/(언리얼 프로젝트명)/PersistentDownload
	// 실제로  C:\UnrealProjects\HttpProject\Saved\PersistentDownloadDir 에 저장.

	FFileHelper::SaveArrayToFile(CompressedData,*imagePath);
}

