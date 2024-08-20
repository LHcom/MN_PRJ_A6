#include "../LHJ/ApiActor.h"
#include "HttpModule.h"


AApiActor::AApiActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AApiActor::BeginPlay()
{
	Super::BeginPlay();
}

void AApiActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AApiActor::ReqPostText(const FString& FileName, const TArray64<uint8>& FileBin)
{
	// HTTP 모듈 인스턴스 가져오기
	FHttpModule& HttpModule = FHttpModule::Get();
	// HTTP 요청 생성
	TSharedRef<IHttpRequest> req = HttpModule.CreateRequest();
	// 요청 URL 설정
	req->SetURL(ApiUrl);
	// 요청 방식 설정 (GET)
	req->SetVerb("POST");
	// 요청 헤더 설정 (선택 사항)
	req->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data"));
	AppendContentBody(req, FileName, FileBin);
	
	// 응답 처리 콜백 바인딩
	req->OnProcessRequestComplete().BindUObject(this, &AApiActor::OnResPostText);
	// 요청 전송
	req->ProcessRequest();
}

void AApiActor::AppendContentBody(TSharedRef<IHttpRequest>& req, const FString& FileName, const TArray64<uint8>& FileBin)
{
	FString body;
	body.Append(TEXT("Content-Disposition: form-data; name=\"profile_image\"; filename=\"ProfileImage.png\"\r\n"));
	body.Append(TEXT("Content-Type: application/octet-stream\r\n\r\n"));

	// 파일 데이터 추가
	for (uint8 Byte : FileBin)
	{
		body.AppendChar((TCHAR)Byte);
	}
	body.Append(TEXT("\r\n"));

	body.Append(TEXT("Content-Disposition: form-data; name=\"imgData\"\r\n\r\n"));
	body.Append(FString::Printf(TEXT("%s\r\n"), *FileName));
	
	req->SetContentAsString(body);
}

ParsingValue AApiActor::ParsingJsonValue(const FString& json)
{
	// 리더기를 만들고
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	// 파싱 결과를 담을 변수 선언
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	// 해석을 한다.
	ParsingValue stParsingValue;
	if (FJsonSerializer::Deserialize(reader, result))
	{
		if (result->HasField(TEXT("returnCode")))
			stParsingValue.returnCode = result->GetIntegerField(TEXT("returnCode"));
		if (result->HasField(TEXT("returnMsg")))
			stParsingValue.returnMsg = result->GetStringField(TEXT("returnMsg"));

		// 리턴 코드가 200일 경우에만 진행
		if (stParsingValue.returnCode == 200)
		{
			if (result->HasField(TEXT("imgTitle")))
				stParsingValue.imgTitle = result->GetStringField(TEXT("imgTitle"));
			if (result->HasField(TEXT("recogMsg")))
				stParsingValue.recogMsg = result->GetStringField(TEXT("recogMsg"));
			if (result->HasField(TEXT("mnStatus")))
				stParsingValue.mnStatus = result->GetStringField(TEXT("mnStatus")                                             );
		}
	}
	return stParsingValue;
}

void AApiActor::OnResPostText(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// parsing
	FString Result = Response->GetContentAsString();
	ParsingValue parsingStruct = ParsingJsonValue(Result);
}
