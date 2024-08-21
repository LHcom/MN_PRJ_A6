// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "PlayerControl.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (player == nullptr)
	{
		//1. 소유한 폰 얻어오기
		auto OwnerPawn = TryGetPawnOwner();
		//2. 플레이어로 캐스팅하기
		player = Cast<APlayerControl>(OwnerPawn);
	}
		//캐스팅이 성공했다면
	if (player) {
		//3. 이동속도가 필요
		FVector velocity = player->GetVelocity();
		//4. 플레이어의 전방 벡터가 필요
		FVector forwardVector = player->GetActorForwardVector();
		//5. 스피드에 값(내적)할당하기
		Speed = FVector::DotProduct(forwardVector, velocity);
		GEngine->AddOnScreenDebugMessage(-1, DeltaSeconds, FColor::Green, FString::Printf(TEXT("%s %s"), *velocity.ToString(), *forwardVector.ToString()));
		//6. 좌우 속도 할당하기
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

	}
}
