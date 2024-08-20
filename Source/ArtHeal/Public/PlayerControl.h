// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerControl.generated.h"

UCLASS()
class ARTHEAL_API APlayerControl : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp; //컴포넌트 속성추가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	//좌우 회전 입력 처리
	void Turn(const struct FInputActionValue& inputValue);
	//상하 회전 입력처리
	void LookUp(const struct FInputActionValue& inputValue);

	//이동속도
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600.0f;
	//이동방향
	FVector direction;

	void Move(const struct FInputActionValue& inputValue);

	//입력 관련 속성
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* imc_Player;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_LookUp;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Turn;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ia_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ia_Paint;

	void PlayerMove(float DeltaTime);

	UFUNCTION()
	void Paint();
	
	UPROPERTY(EditAnywhere, Category = Mysetting)
	int fireTraceDistance = 1800;


};
