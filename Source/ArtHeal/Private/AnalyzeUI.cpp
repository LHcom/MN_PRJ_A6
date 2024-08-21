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

void UAnalyzeUI::NativeConstruct()
{
	Super::NativeConstruct();
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

void UAnalyzeUI::SetDrawingImage()
{
	
}


