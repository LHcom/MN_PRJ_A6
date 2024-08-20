// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DrawingUI.generated.h"

/**
 * 
 */
UCLASS()
class ARTHEAL_API UDrawingUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SendButton; //�м� ������ ��ư

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ArtTitle;

	UFUNCTION()
	void OnMyClickSend(); //�м��� ������ �Լ�

	UPROPERTY()
	class APlayerControl* player;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* IMG_Painted;

	UPROPERTY(EditAnywhere, Category = "Paint")
	UMaterialInterface* IMG_PaintMat;

	UPROPERTY(EditAnywhere, Category = "Paint")
	UMaterialInstanceDynamic* TempMat;
	
	void GetImageMat();

	void SetImageMat();

	void SetArtTitle();

	FString GetRandomTopic();
	
	//URL
	FString URL;

	FString Key;
	
};
