// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class ARTHEAL_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerAnim)
	float Speed;

	//�÷��̾� �¿� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerAnim)
	float direction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PlayerAnim)
	class APlayerControl* player;
	
};
