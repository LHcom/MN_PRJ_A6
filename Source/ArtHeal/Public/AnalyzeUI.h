// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnalyzeUI.generated.h"

/**
 * 
 */
UCLASS()
class ARTHEAL_API UAnalyzeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	class AApiActor* ApiActor;

	// JSON �����͸� �޾� �ؽ�Ʈ ��Ͽ� �����ϴ� �Լ�
	void SetAnalysisText(const FString& ImgTitle, const FString& RecogMsg);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ImgTitleText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnalysisText;

	UPROPERTY(meta = (BindWidget))
	class UImage* DrawingImage; //���� �׸� �׸� �ҷ��� �̹���

	UPROPERTY(EditAnywhere, Category = "Paint")
	UMaterialInterface* IMG_PaintMat;

	UPROPERTY(EditAnywhere, Category = "Paint")
	UMaterialInstanceDynamic* TempMat;


	UFUNCTION()
	void SetDrawingImage();

	UFUNCTION()
	void GetDrawingImage();
};
