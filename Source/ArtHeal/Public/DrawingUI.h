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
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_0;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_2;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_3;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_4;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_5;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_6;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_7;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_8;

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
	
	UFUNCTION()
	void OnClickedColorBtn_0();
	UFUNCTION()
	void OnClickedColorBtn_1();
	UFUNCTION()
	void OnClickedColorBtn_2();
	UFUNCTION()
	void OnClickedColorBtn_3();
	UFUNCTION()
	void OnClickedColorBtn_4();
	UFUNCTION()
	void OnClickedColorBtn_5();
	UFUNCTION()
	void OnClickedColorBtn_6();
	UFUNCTION()
	void OnClickedColorBtn_7();
	UFUNCTION()
	void OnClickedColorBtn_8();
	
	UPROPERTY()
	class APaintTarget* PaintTarget;
};

