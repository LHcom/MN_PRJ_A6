// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorTrigger.generated.h"

UCLASS()
class ARTHEAL_API ADoorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* Trigger;

	UPROPERTY(EditDefaultsOnly)
	class ALevelSequenceActor* LevelActor;

	UPROPERTY(EditDefaultsOnly)
	class ULevelSequence* DoorSequence;

	UPROPERTY(EditDefaultsOnly)
	class ULevelSequencePlayer* LevelPlayer;

};
