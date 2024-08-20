// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControl.h"
#include "GameFramework/SpringArmComponent.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <InputTriggers.h>
#include<Camera/CameraComponent.h>
#include "GameFramework/Character.h"

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


