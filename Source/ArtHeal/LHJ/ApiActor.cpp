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

TArray<uint8> FStringToUint8(const FString& InString)
{
	TArray<uint8> OutBytes;
 
	// Handle empty strings
	if (InString.Len() > 0)
	{
		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
	}
 
	return OutBytes;
}
 
FString AApiActor::AddData(FString Name, FString Value) {
	return FString(TEXT("\r\n"))
		+ BoundaryBegin
		+ FString(TEXT("Content-Disposition: form-data; name=\""))
		+ Name
		+ FString(TEXT("\"\r\n\r\n"))
		+ Value;
}

void AApiActor::ProcessResponse(FString ResponseContent)
{
	// Here you can process the response body
	UE_LOG(LogTemp, Error, TEXT("Response: %s"), *ResponseContent);
}

void AApiActor::ReqPostText(const FString& FullFilePath, const TArray64<uint8>& FileBin)
{
	FString FileName = FPaths::GetCleanFilename(FullFilePath);
 
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> HttpRequest = HttpModule.CreateRequest();
 
	// We set the api URL
	HttpRequest->SetURL(ApiUrl);
	
	// We set verb of the request (GET/PUT/POST)
	HttpRequest->SetVerb(TEXT("POST")); 
 
	// Create a boundary label, for the header
	BoundaryLabel = FString(TEXT("e543322540af456f9a3773049ca02529-")) + FString::FromInt(FMath::Rand());
	// boundary label for begining of every payload chunk 
	BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));
	// boundary label for the end of payload
	BoundaryEnd = FString(TEXT("\r\n--")) + BoundaryLabel + FString(TEXT("--\r\n"));
 
	// Set the content-type for server to know what are we going to send
	HttpRequest->SetHeader(TEXT("Content-Type"), FString(TEXT("multipart/form-data; boundary=")) + BoundaryLabel);
 
	// This is binary content of the request
	TArray<uint8> CombinedContent; 
 
	// First, we add the boundary for the file, which is different from text payload
	FString FileBoundaryString = FString(TEXT("\r\n"))
		+ BoundaryBegin
		+ FString(TEXT("Content-Disposition: form-data; name=\"file\"; filename=\""))
		+ FileName + "\"\r\n"
		+ "Content-Type: image/jpeg"
		+ FString(TEXT("\r\n\r\n"));
		
	// Notice, we convert all strings into uint8 format using FStringToUint8
	CombinedContent.Append(FStringToUint8(FileBoundaryString));
	
	// Append the file data
	CombinedContent.Append(FileBin);
	
	// Let's add couple of text values to the payload
	CombinedContent.Append(FStringToUint8(AddData("imgData", FileName)));
 
	// Finally, add a boundary at the end of the payload
	CombinedContent.Append(FStringToUint8(BoundaryEnd));
 
	// Set the request content
	HttpRequest->SetContent(CombinedContent); 
 
	// Hook a lambda(anonymous function) to when we receive a response
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &AApiActor::OnResPostText);
	// HttpRequest->OnProcessRequestComplete().BindLambda(
	// 	[this](
	// 		FHttpRequestPtr pRequest,
	// 		FHttpResponsePtr pResponse,
	// 		bool connectedSuccessfully) mutable {
	// 			UE_LOG(LogTemp, Error, TEXT("Connection."));
 //
	// 			if (connectedSuccessfully) {
	// 				ProcessResponse(pResponse->GetContentAsString());
	// 			}
	// 			else {
	// 				switch (pRequest->GetStatus()) {
	// 				case EHttpRequestStatus::Failed_ConnectionError:
	// 					UE_LOG(LogTemp, Error, TEXT("Connection failed."));
	// 				default:
	// 					UE_LOG(LogTemp, Error, TEXT("Request failed."));
	// 				}
	// 			}
	// 	});
 
	// Send the request 
	HttpRequest->ProcessRequest();
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
