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

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnalysisText; //분석글 텍스트

	UPROPERTY(meta = (BindWidget))
	class UImage* StateImage; //상태 표현 이미지

	UPROPERTY(meta = (BindWidget))
	class UImage* DrawingImage; //내가 그린 그림 불러올 이미지, 건드리지 말것

	UFUNCTION()
	void OnMyClickRecieve(); //분석 글 받기

	void SetAnalysisText(FString Log); //분석글 텍스트 설정 함수

	void SetApiActor(class AApiActor* actor); 

	void SetDrawingImage();
	
};
