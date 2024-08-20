// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/PaintTarget.h"

#include "DrawingUI.h"
#include "ImageUtils.h"
#include "PlayerControl.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
APaintTarget::APaintTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent =CreateDefaultSubobject<USceneComponent>(TEXT("USceneComponent"));
	SetRootComponent(sceneComponent);
	
	
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	staticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APaintTarget::BeginPlay()
{
	Super::BeginPlay();

	//만들고 CLEAR 해서 넣어줌 .

	APlayerControl*  Player =Cast<APlayerControl>( GetWorld()->GetFirstPlayerController()->GetCharacter());

	
	
	CRT_PaintMask = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(),1024,1024,RTF_RGBA32f);
	
	UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(),CRT_PaintMask,FLinearColor(1,1,1,1));

	
	OriginMat =staticMesh->GetMaterial(0);
	
	TempMat =UMaterialInstanceDynamic::Create(OriginMat,this);

	staticMesh->SetMaterial(0,TempMat);
	
	TempMat->SetTextureParameterValue(FName(TEXT("PaintTexture")),CRT_PaintMask);

	// ui 도 다이내믹 matrial 으로 설정
	//DrawingUI->GetImageMat();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this,Player]()
	{
		DrawingUI = Player->DrawingUI;


		DrawingUI->SetImageMat();

		DrawingUI->TempMat->SetTextureParameterValue(FName(TEXT("PaintTexture")), CRT_PaintMask);
	}, 1.f, false);
	
	
	
	
}

// Called every frame
void APaintTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APaintTarget::Painted(int num,FVector2D DrawLocation,float brushSize)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s(%d) : press Fire"), *FString(__FUNCTION__), __LINE__);
	UMaterialInterface* brushTexture;
	FVector2D brushLocation;
	
	brushTexture = BrushMatArray[num];
	//UKismetRenderingLibrary::DrawMaterialToRenderTarget(GetWorld(),CRT_DamageMask,MI_PaintTexture);
	
	UCanvas* Canvas;
	FVector2D size;
	FDrawToRenderTargetContext context;
	
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(),CRT_PaintMask,Canvas,size,context);
	
	brushLocation.X = (size.X*DrawLocation.X)-brushSize*0.5f;
	brushLocation.Y = (size.Y*DrawLocation.Y)-brushSize*0.5f;
	Canvas->K2_DrawMaterial(brushTexture,brushLocation,FVector2D(brushSize),FVector2D(0,0),FVector2D(1,1));

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(),context);

}

void APaintTarget::SaveTexture(UTextureRenderTarget2D* TexRT)
{
	// 호출히면 저장하는 함수
	//FImageUtils::ExportRenderTarget2DAsPNG(TexRT, FArchive& Ar);
}

