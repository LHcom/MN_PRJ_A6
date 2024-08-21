// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorTrigger.h"
#include "Components/BoxComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

// Sets default values
ADoorTrigger::ADoorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Trigger component 생성 및 초기화
	//Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	//Trigger->SetBoxExtent(FVector(30.0f, 30.0f, 30.0f));
	//Trigger->SetCollisionProfileName(TEXT("Trigger"));
	//RootComponent = Trigger;

	//// LevelSequenceActor와 DoorSequence 초기화 (필요 시)
	//LevelActor = nullptr;
	//DoorSequence = nullptr;

	//// LevelSequence 찾기
	//ConstructorHelpers::FObjectFinder<ULevelSequence> LS(TEXT("/Game/Bada/sequence/DoorAnim.DoorAnim"));
	//if (LS.Succeeded()) {
	//	DoorSequence = LS.Object;
	//}

}

// Called when the game starts or when spawned
void ADoorTrigger::BeginPlay()
{
	Super::BeginPlay();

	/*FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;
	Settings.bPauseAtEnd = true;

	if (DoorSequence)
	{
		LevelPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), DoorSequence, Settings, LevelActor);
		if (!LevelPlayer)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create Level Sequence Player."));
			return;
		}
	}*/
	
}

// Called every frame
void ADoorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ADoorTrigger::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorBeginOverlap(OtherActor);
//
//	UE_LOG(LogTemp, Warning, TEXT("d1"));
//
//	if (LevelPlayer) {
//		UE_LOG(LogTemp, Warning, TEXT("d2"));
//		LevelPlayer->Play();
//	}
//}
//
//void ADoorTrigger::NotifyActorEndOverlap(AActor* OtherActor)
//{
//	Super::NotifyActorEndOverlap(OtherActor);
//	UE_LOG(LogTemp, Warning, TEXT("c1"));
//	if (LevelPlayer) {
//		UE_LOG(LogTemp, Warning, TEXT("c2"));
//		LevelPlayer->PlayReverse();
//	}
//}

