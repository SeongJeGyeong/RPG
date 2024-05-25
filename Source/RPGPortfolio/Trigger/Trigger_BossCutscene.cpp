// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_BossCutscene.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../UI/UI_Base.h"

void ATrigger_BossCutscene::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATrigger_BossCutscene::BeginOverlap);
}

void ATrigger_BossCutscene::BeginOverlap(AActor* _TriggerActor, AActor* _OtherActor)
{
	if ( IsValid(m_LevelSeq) )
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

			//ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			//if ( IsValid(GameMode) )
			//{
			//	GameMode->GetMainHUD()->SetVisibility(ESlateVisibility::Hidden);
			//}
			m_SeqPlayer->Play();
		}
	}
}

void ATrigger_BossCutscene::EndLevelSequence()
{
	Destroy();
}
