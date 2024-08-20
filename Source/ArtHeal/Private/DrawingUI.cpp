// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingUI.h"

#include "PlayerControl.h"
#include "Components/Button.h"
#include "ArtHeal/LHJ/ApiActor.h"

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
