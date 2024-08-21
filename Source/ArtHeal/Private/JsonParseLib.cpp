// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"

ParsingValue UJsonParseLib::JsonParse(const FString& Json)
{
	// �����⸦ �����
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(Json);
	// �Ľ� ����� ���� ������ �����ϰ�
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	ParsingValue parsingStruct;

	// JSON�� �Ľ��Ѵ�.
	if (FJsonSerializer::Deserialize(reader, result))
	{
		// JSON���� �����͸� �����Ͽ� ����ü�� �Ҵ�
		if (result->HasField(TEXT("returnCode")))
			parsingStruct.returnCode = result->GetIntegerField(TEXT("returnCode"));
		if (result->HasField(TEXT("returnMsg")))
			parsingStruct.returnMsg = result->GetStringField(TEXT("returnMsg"));

		// ���� �ڵ尡 200�� ��쿡�� �߰� ������ �Ľ�
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

	// �Ľ̵� �����͸� ��ȯ
	return parsingStruct;
}

FString UJsonParseLib::MakeJson(const TMap<FString, FString> source)
{
	// source�� JsonObject �������� �����
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());
	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer�� ����
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);

	// JsonObject�� ���ڵ��ϰ�
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);

	// ���ڵ� ����� ��ȯ�Ѵ�.
	return json;
}
