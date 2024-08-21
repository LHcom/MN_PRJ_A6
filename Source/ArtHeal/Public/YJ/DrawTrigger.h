// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawTrigger.generated.h"

UCLASS()
class ARTHEAL_API ADrawTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UStaticMeshComponent* StaticComp;

	UFUNCTION()
	void OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	class APlayerControl* Player;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ACameraActor* CameraActor;

};
