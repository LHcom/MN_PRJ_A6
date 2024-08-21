// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "ApiActor.generated.h"

struct ParsingValue
{
	int32 returnCode;	// 리턴 코드
	FString returnMsg;	// 리턴 메세지
	FString imgTitle;	// 이미지 제목
	FString recogMsg;	// 인식 내용
};

UCLASS()
class ARTHEAL_API AApiActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AApiActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//FString ApiUrl = "http://172.16.16.169:8080/api/v1/files/upload"; // api url
	FString ApiUrl = "http://192.168.1.18:8080/api/v1/files/upload"; // api url

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ReqPostText(const FString& FullFilePath, const TArray64<uint8>& FileBin);
	void OnResPostText(FHttpRequestPtr Request, FHttpResponsePtr Response,
	                   bool bConnectedSuccessfully);

	ParsingValue ParsingJsonValue(const FString& json);

	// File Upload
	FString BoundaryLabel = FString();
	FString BoundaryBegin = FString();
	FString BoundaryEnd = FString();
	FString AddData(FString Name, FString Value);
	void ProcessResponse(FString ResponseContent);
};
