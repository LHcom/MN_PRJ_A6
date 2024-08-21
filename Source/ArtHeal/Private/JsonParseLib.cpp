// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"

ParsingValue UJsonParseLib::JsonParse(const FString& Json)
{
	// 리더기를 만들고
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(Json);
	// 파싱 결과를 담을 변수를 선언하고
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	ParsingValue parsingStruct;

	// JSON을 파싱한다.
	if (FJsonSerializer::Deserialize(reader, result))
	{
		// JSON에서 데이터를 추출하여 구조체에 할당
		if (result->HasField(TEXT("returnCode")))
			parsingStruct.returnCode = result->GetIntegerField(TEXT("returnCode"));
		if (result->HasField(TEXT("returnMsg")))
			parsingStruct.returnMsg = result->GetStringField(TEXT("returnMsg"));

		// 리턴 코드가 200일 경우에만 추가 데이터 파싱
		if (parsingStruct.returnCode == 200)
		{
			if (result->HasField(TEXT("imgTitle")))
				parsingStruct.imgTitle = result->GetStringField(TEXT("imgTitle"));
			if (result->HasField(TEXT("recogMsg")))
				parsingStruct.recogMsg = result->GetStringField(TEXT("recogMsg"));
			/*if (result->HasField(TEXT("mnStatus")))
				parsingStruct.mnStatus = result->GetStringField(TEXT("mnStatus"));*/
		}
	}

	// 파싱된 데이터를 반환
	return parsingStruct;
}

FString UJsonParseLib::MakeJson(const TMap<FString, FString> source)
{
	// source를 JsonObject 형식으로 만들고
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());
	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer를 만들어서
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);

	// JsonObject를 인코딩하고
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	// 인코딩 결과를 반환한다.
	return json;
}
