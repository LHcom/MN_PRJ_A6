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
	class UButton* SendButton; //분석 보내는 버튼

	UFUNCTION()
	void OnMyClickSend(); //분석글 보내는 함수

	//URL
	FString URL;

	FString Key;
	
};
