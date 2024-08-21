// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingUI.h"

#include "PlayerControl.h"
#include "Components/Button.h"
#include "ArtHeal/LHJ/ApiActor.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AnalyzeUI.h"

void UDrawingUI::NativeConstruct()
{
	Super::NativeConstruct();

	//버튼연결
	SendButton->OnClicked.AddDynamic(this, &UDrawingUI::OnMyClickSend);

	player = Cast<APlayerControl>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	Button_0->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_0);
	Button_1->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_1);
	Button_2->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_2);
	Button_3->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_3);
	Button_4->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_4);
	Button_5->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_5);
	Button_6->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_6);
	Button_7->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_7);
	Button_8->OnClicked.AddDynamic(this, &UDrawingUI::OnClickedColorBtn_8);

	PaintTarget =player->Paintable;
}

void UDrawingUI::OnMyClickSend()
{
	FString FullURL;
	//= FString::Printf(TEXT("%s?serviceKey=%s&pageNo=%d&numOfRows=%d"));
	player->SaveTexture();
    PlayAnimationForward(Loading);

    //그다음 분석 UI로 넘어가도록 코드 작성
    player->AnalyzeUI->SetVisibility(ESlateVisibility::Visible);
    player->DrawingUI->SetVisibility(ESlateVisibility::Hidden);


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

void UDrawingUI::OnClickedColorBtn_0()
{
	// 버튼을 누르면 브러쉬의 mat 변수에 값을 변경하여 색깔을 변경하고 싶다
	// 캐릭터가 그릴때 브러쉬 넘버를 보내기때문에
	player->SetBrushNum(0);
	
}

void UDrawingUI::OnClickedColorBtn_1()
{
	player->SetBrushNum(1);
}

void UDrawingUI::OnClickedColorBtn_2()
{
	player->SetBrushNum(2);
}

void UDrawingUI::OnClickedColorBtn_3()
{
	player->SetBrushNum(3);
}

void UDrawingUI::OnClickedColorBtn_4()
{
	player->SetBrushNum(4);
}

void UDrawingUI::OnClickedColorBtn_5()
{
	player->SetBrushNum(5);
}

void UDrawingUI::OnClickedColorBtn_6()
{
	player->SetBrushNum(6);
}

void UDrawingUI::OnClickedColorBtn_7()
{
	player->SetBrushNum(7);
}

void UDrawingUI::OnClickedColorBtn_8()
{
	player->SetBrushNum(8);
}
