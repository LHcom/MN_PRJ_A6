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

	UFUNCTION()
	void OnMyClickSend(); //�м��� ������ �Լ�

	UPROPERTY()
	class APlayerControl* player;

	//URL
	FString URL;

	FString Key;
	
};
