// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingUI.h"

#include "PlayerControl.h"
#include "Components/Button.h"
#include "ArtHeal/LHJ/ApiActor.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDrawingUI::NativeConstruct()
{
	Super::NativeConstruct();

	//버튼연결
	SendButton->OnClicked.AddDynamic(this, &UDrawingUI::OnMyClickSend);

	player = Cast<APlayerControl>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
}

void UDrawingUI::OnMyClickSend()
{
	FString FullURL;
	//= FString::Printf(TEXT("%s?serviceKey=%s&pageNo=%d&numOfRows=%d"));
	player->SaveTexture();
}

void UDrawingUI::GetImageMat()
{
	 IMG_PaintMat =Cast<UMaterialInterface>(IMG_Painted->GetBrush().GetResourceObject());
}

void UDrawingUI::SetImageMat()
{
	TempMat =UMaterialInstanceDynamic::Create(IMG_PaintMat,this);
	IMG_Painted->SetBrushFromMaterial(TempMat);	
}

void UDrawingUI::SetArtTitle()
{
    FString RandomTopic = GetRandomTopic();
    UE_LOG(LogTemp, Warning, TEXT("Selected Topic: %s"), *RandomTopic);

    ArtTitle->SetText(FText::FromString(RandomTopic));


}

FString UDrawingUI::GetRandomTopic()
{
    // 주제들을 배열로 저장
    TArray<FString> Topics = {
        TEXT("나무 그리기"),
        TEXT("집 그리기"),
        TEXT("길 그리기"),
        TEXT("사과나무에서 사과 따는 사람"),
        TEXT("빗속의 사람")
    };

    // 사용된 주제를 저장할 배열
    TArray<FString> UsedTopics;

    // Topics 배열이 비어있는지 확인
    if (Topics.Num() == 0)
    {
        // 모든 주제가 사용되었다면 다시 초기화
        if (UsedTopics.Num() > 0)
        {
            // 사용된 주제들을 원래 배열로 복사
            Topics = UsedTopics;
            // 사용된 주제 배열 초기화
            UsedTopics.Empty();
        }
        else
        {
            // 주제가 없을 때 처리
            UE_LOG(LogTemp, Error, TEXT("No topics available."));
            return TEXT("Error: No topics available.");
        }
    }

    // 인덱스가 유효한지 확인
    if (Topics.Num() > 0)
    {
        // 랜덤으로 주제 선택
        int32 RandomIndex = FMath::RandRange(0, Topics.Num() - 1);
        FString SelectedTopic = Topics[RandomIndex];

        // 선택된 주제를 사용된 주제 배열로 이동
        UsedTopics.Add(SelectedTopic);
        Topics.RemoveAt(RandomIndex);

        // 선택된 주제 리턴
        return SelectedTopic;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("No topics available after reset."));
        return TEXT("Error: No topics available after reset.");
    }
}
