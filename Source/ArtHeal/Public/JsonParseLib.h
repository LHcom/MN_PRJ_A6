// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLib.generated.h"

struct ParsingValue
{
	int32 returnCode;	// 리턴 코드
	FString returnMsg;	// 리턴 메세지
	FString imgTitle;	// 이미지 제목
	FString recogMsg;	// 인식 내용
	FString mnStatus;	// 심리 상태
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
