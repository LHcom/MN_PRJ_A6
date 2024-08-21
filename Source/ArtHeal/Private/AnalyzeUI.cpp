// Fill out your copyright notice in the Description page of Project Settings.


#include "AnalyzeUI.h"
#include "../LHJ/ApiActor.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Misc/FileHelper.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "Components/Button.h"
#include "PlayerControl.h"

void UAnalyzeUI::NativeConstruct()
{
	Super::NativeConstruct();

	QuitButton->OnClicked.AddDynamic(this, &UAnalyzeUI::OnClickHidden);
}

void UAnalyzeUI::SetAnalysisText(const FString& ImgTitle, const FString& RecogMsg)
{
	if (ImgTitleText)
	{
		ImgTitleText->SetText(FText::FromString(ImgTitle));
	}

	if (AnalysisText)
	{
		AnalysisText->SetText(FText::FromString(RecogMsg));
	}
}

void UAnalyzeUI::GetDrawingImage()
{
	
}

void UAnalyzeUI::OnClickHidden()
{
	SetVisibility(ESlateVisibility::Hidden);

	player = Cast<APlayerControl>(GetWorld()->GetFirstPlayerController()->GetPawn());
	player->pc->SetShowMouseCursor(false);
	player->pc->SetInputMode(FInputModeGameOnly());

}

void UAnalyzeUI::SetDrawingImage()
{
	TempMat =UMaterialInstanceDynamic::Create(IMG_PaintMat,this);
	DrawingImage->SetBrushFromMaterial(TempMat);	
}


