// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/DrawTrigger.h"

#include "DrawingUI.h"
#include "PlayerControl.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADrawTrigger::ADrawTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComp);

	StaticComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADrawTrigger::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&ADrawTrigger::OnBoxBeginOverlap);

	CameraActor =Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(this,ACameraActor::StaticClass()));
}

// Called every frame
void ADrawTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrawTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayerControl>(OtherActor);
	
	if (Player) //그림그리기 이제부터 시작
	{
		Player->bPainting = true;

		//마우스커서 보이기
		Player->ShowMouseCursor(true);
		Player->DrawingUI->SetVisibility(ESlateVisibility::Visible);
		//Player->pc->SetInputMode(FInputModeGameAndUI());
		//Player->pc->SetInputMode(FInputModeUIOnly());
		//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Player->pc,nullptr)
		Player->pc->SetViewTargetWithBlend(CameraActor);
		
	}
}

