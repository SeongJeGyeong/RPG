// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction_Lever.h"
#include "Components/BoxComponent.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../UI/UI_Base.h"
#include "../System/FadeViewportClient.h"
#include "../Characters/Player_Base_Knight.h"

// Sets default values
AInteraction_Lever::AInteraction_Lever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	SetRootComponent(m_Mesh);
	m_Trigger->SetupAttachment(m_Mesh);

	m_Trigger->SetCollisionProfileName(TEXT("InteractionTrigger"));
	m_Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AInteraction_Lever::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);

	tCommand_Key = FText::FromString(TEXT("F"));
	tCommand_Name = FText::FromString(TEXT("당긴다"));
}

// Called when the game starts or when spawned
void AInteraction_Lever::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteraction_Lever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteraction_Lever::Interaction(AActor* _InteractedActor)
{
	m_TriggeredActor = Cast<APlayer_Base_Knight>(_InteractedActor);
	if ( !IsValid(m_LevelSeq) || !IsValid(m_TriggeredActor) )
	{
		UE_LOG(LogTemp, Error, TEXT("시퀀스 재생 오브젝트와 상호작용 실패"));
		return;
	}

	FMovieSceneSequencePlaybackSettings Settings = {};
	Settings.bRestoreState = false;
	Settings.bDisableMovementInput = true;
	ALevelSequenceActor* pSequenceActor = nullptr;

	m_SeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), m_LevelSeq, Settings, pSequenceActor);
	if ( IsValid(m_SeqPlayer) )
	{
		// 레벨시퀀스 종료시 호출할 Delegate 등록
		m_SeqPlayer->OnFinished.AddDynamic(this, &AInteraction_Lever::EndLevelSequence);
		ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if ( IsValid(GameMode) )
		{
			GameMode->GetMainHUD()->SetVisibility(ESlateVisibility::Hidden);
			GameMode->GetMainHUD()->ShowMainMessageUI(false);
		}
		m_TriggeredActor->Play_PlayerMontage(EPlayerMontage::ACTION_PROP);
		m_TriggeredActor->DisableInput(NULL);
		m_SeqPlayer->Play();
	}
}

void AInteraction_Lever::EndLevelSequence()
{
	m_Trigger->DestroyComponent();
	
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		GameMode->GetMainHUD()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	const UWorld* World = GetWorld();
	if ( World )
	{
		UFadeViewportClient* GameViewportClient = Cast<UFadeViewportClient>(World->GetGameViewport());
		if ( GameViewportClient )
		{
			GameViewportClient->Fade(1.f, false);
		}
	}

	if ( IsValid(m_TriggeredActor) )
	{
		m_TriggeredActor->EnableInput(NULL);
		m_TriggeredActor = nullptr;
	}
}