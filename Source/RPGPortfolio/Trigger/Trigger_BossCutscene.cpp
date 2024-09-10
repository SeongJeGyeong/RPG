// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_BossCutscene.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_FadeScreen.h"
#include "../Monsters/Boss_Base.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../System/FadeViewportClient.h"
#include "../Manager/GISubsystem_SoundMgr.h"

void ATrigger_BossCutscene::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATrigger_BossCutscene::BeginOverlap);
}

void ATrigger_BossCutscene::BeginOverlap(AActor* _TriggerActor, AActor* _OtherActor)
{
	if (IsValid(m_LevelSeq))
	{
		FMovieSceneSequencePlaybackSettings Settings = {};
		Settings.bRestoreState = false;
		Settings.bDisableMovementInput = true;
		ALevelSequenceActor* pSequenceActor = nullptr;

		if (!IsValid(m_SeqPlayer))
		{
			m_SeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), m_LevelSeq, Settings, pSequenceActor);

			// 레벨시퀀스 종료시 호출할 Delegate 등록
			m_SeqPlayer->OnFinished.AddDynamic(this, &ATrigger_BossCutscene::EndLevelSequence);
			ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			if ( IsValid(GameMode) )
			{
				GameMode->GetMainHUD()->SetVisibility(ESlateVisibility::Hidden);
			}
			m_SeqPlayer->Play();
		}
	}

	GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>()->BGMLoadAsync();
}

void ATrigger_BossCutscene::EndLevelSequence()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( IsValid(GameMode) )
	{
		GameMode->GetMainHUD()->SetVisibility(ESlateVisibility::Visible);
		GameMode->PlayBGM(true);

		//GameMode->GetFadeUI()->FadeIn(1.f);
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

	TArray<AActor*> OutActorsArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoss_Base::StaticClass(), OutActorsArr);
	if (OutActorsArr.Num() > 0)
	{
		AAIController* pAIController = Cast<AAIController>(OutActorsArr[0]->GetInstigatorController());
		if (IsValid(pAIController))
		{
			pAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("bStop"), false);
		}
	}

	Destroy();
}
