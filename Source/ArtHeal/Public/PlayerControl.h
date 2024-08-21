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
	class USpringArmComponent* springArmComp; //������Ʈ �Ӽ��߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* tpsCamComp;

	//�¿� ȸ�� �Է� ó��
	void Turn(const struct FInputActionValue& inputValue);
	//���� ȸ�� �Է�ó��
	void LookUp(const struct FInputActionValue& inputValue);

	//�̵��ӵ�
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 50.0f;
	//�̵�����
	FVector direction;

	void Move(const struct FInputActionValue& inputValue);

	//�Է� ���� �Ӽ�
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
	
	UPROPERTY(EditAnywhere, Category = "Paint")
	int fireTraceDistance = 1800;
	
	UFUNCTION(BlueprintCallable)
	void SaveTexture();
	
	UPROPERTY()
	class APaintTarget* Paintable;

	UPROPERTY()
	class AApiActor* ApiActor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Paint")
	TSubclassOf<class UUserWidget> DrawingUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDrawingUI* DrawingUI;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Paint")
	TSubclassOf<class UUserWidget> AnalyzeUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnalyzeUI* AnalyzeUI;

	UPROPERTY()
	class UPlayerAnim* anim;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 brushNum;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float brushSize=70.f;
	UFUNCTION()
	void SetBrushNum(int32 num);
	UFUNCTION()
	void SetBrushSize(float num);
	
	UFUNCTION(BlueprintCallable)
	void SetDrawingUIVisible(bool value);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bPainting=false;
	
	UPROPERTY()
	APlayerController* pc;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector MyVector;

	UFUNCTION(BlueprintCallable)
	void ShowMouseCursor(bool value);
	
	FString GetFormattedDateTime();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ACameraActor* CameraActor;
};
