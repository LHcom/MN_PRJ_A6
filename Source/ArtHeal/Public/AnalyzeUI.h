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

	// JSON 데이터를 받아 텍스트 블록에 설정하는 함수
	void SetAnalysisText(const FString& ImgTitle, const FString& RecogMsg);

	void SetDrawingImage();

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ImgTitleText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AnalysisText;

	UPROPERTY(meta = (BindWidget))
	class UImage* DrawingImage; //내가 그린 그림 불러올 이미지
	
};
