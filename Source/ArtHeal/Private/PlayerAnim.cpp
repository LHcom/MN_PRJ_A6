// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerControl.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player == nullptr)
	{
		//1. ������ �� ������
		auto OwnerPawn = TryGetPawnOwner();
		//2. �÷��̾�� ĳ�����ϱ�
		player = Cast<APlayerControl>(OwnerPawn);
	}
		//ĳ������ �����ߴٸ�
	if (player) {
		//3. �̵��ӵ��� �ʿ�
		FVector velocity = player->GetVelocity();
		//4. �÷��̾��� ���� ���Ͱ� �ʿ�
		FVector forwardVector = player->GetActorForwardVector();
		//5. ���ǵ忡 ��(����)�Ҵ��ϱ�
		Speed = FVector::DotProduct(forwardVector, velocity);
		GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, FString::Printf(TEXT("%s %s"), *velocity.ToString(), *forwardVector.ToString()));
		//6. �¿� �ӵ� �Ҵ��ϱ�
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

	}
}
