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

	void SetDrawingImage();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ImgTitleText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnalysisText;

	UPROPERTY(meta = (BindWidget))
	class UImage* DrawingImage; //���� �׸� �׸� �ҷ��� �̹���
	
};
