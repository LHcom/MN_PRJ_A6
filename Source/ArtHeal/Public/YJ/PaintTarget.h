// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaintTarget.generated.h"

UCLASS()
class ARTHEAL_API APaintTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaintTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	class USceneComponent* sceneComponent;
	
	UPROPERTY(EditAnywhere, Category = "Paint")
	class UStaticMeshComponent* staticMesh;
	
	UPROPERTY(EditAnywhere, Category = "Paint")
	UTextureRenderTarget2D* CRT_PaintMask;

	UPROPERTY(EditAnywhere, Category = "Paint")
	UMaterialInterface* OriginMat;

	UPROPERTY(EditAnywhere, Category = "Paint")
	UMaterialInstanceDynamic* TempMat;

	UPROPERTY(EditAnywhere, Category = "Paint")
	TArray<UMaterialInterface*> BrushMatArray;
	
	void Painted(int num,FVector2D DrawLocation,float brushSize);

	void SaveTexture(UTextureRenderTarget2D* TexRT);

	UPROPERTY()
	class UDrawingUI* DrawingUI;

	UPROPERTY()
	class UAnalyzeUI* AnalyzeUI;
};
