// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLib.generated.h"

struct ParsingValue
{
	int32 returnCode;	// ���� �ڵ�
	FString returnMsg;	// ���� �޼���
	FString imgTitle;	// �̹��� ����
	FString recogMsg;	// �ν� ����
	FString mnStatus;	// �ɸ� ����
};

UCLASS()
class ARTHEAL_API UJsonParseLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "JSON")
	ParsingValue JsonParse(const FString& Json);

	UFUNCTION(BlueprintCallable, Category = "JSON")
	FString MakeJson(const TMap<FString, FString> source);
	
};
