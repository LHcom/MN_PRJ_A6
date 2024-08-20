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
	class UTextBlock* AnalysisText; //�м��� �ؽ�Ʈ

	UPROPERTY(meta = (BindWidget))
	class UImage* StateImage; //���� ǥ�� �̹���

	UPROPERTY(meta = (BindWidget))
	class UImage* DrawingImage; //���� �׸� �׸� �ҷ��� �̹���, �ǵ帮�� ����

	UFUNCTION()
	void OnMyClickRecieve(); //�м� �� �ޱ�

	void SetAnalysisText(FString Log); //�м��� �ؽ�Ʈ ���� �Լ�

	void SetApiActor(class AApiActor* actor); 

	void SetDrawingImage();
	
};
