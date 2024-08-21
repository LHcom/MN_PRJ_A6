// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "ApiActor.generated.h"

struct ParsingValue
{
	int32 returnCode; // 리턴 코드
	FString returnMsg; // 리턴 메세지
	FString imgTitle; // 이미지 제목
	FString recogMsg; // 인식 내용
};


// struct FWavHeader
// {
// 	uint8 ChunkID[4];
// 	uint32 ChunkSize;
// 	uint8 Format[4];
// 	uint8 Subchunk1ID[4];
// 	uint32 Subchunk1Size;
// 	uint16 AudioFormat;
// 	uint16 NumChannels;
// 	uint32 SampleRate;
// 	uint32 ByteRate;
// 	uint16 BlockAlign;
// 	uint16 BitsPerSample;
// 	uint8 Subchunk2ID[4];
// 	uint32 Subchunk2Size;
// };

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
	FString ApiUrl = "http://172.16.16.169:8080/api/v1/ai/upload"; // api url
	//FString ApiUrl = "http://172.16.16.169:8080/api/v1/files/upload"; // api url
	//FString ApiUrl = "http://192.168.1.18:8080/api/v1/files/upload"; // api url

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

	// Wav Download
	// UFUNCTION(BlueprintCallable)
	// void WavFileDownload(FString DownloadURL, FString SaveFullPath);
	// void OnResDownloadWav(FHttpRequestPtr Request, FHttpResponsePtr Response,
	// 				   bool bConnectedSuccessfully);

	//UFUNCTION(BlueprintCallable)
	//void PlaySoundFromFile(const FString& FilePath);

	// UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	// void LoadWavFile(const FString& FilePath);
	// FString SaveWavPath;
};
