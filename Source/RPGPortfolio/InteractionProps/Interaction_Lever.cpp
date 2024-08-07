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

void AInteraction_Lever::Interaction()
{
	UE_LOG(LogTemp, Warning, TEXT("레버 돌리기"));
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
			m_SeqPlayer->OnFinished.AddDynamic(this, &AInteraction_Lever::EndLevelSequence);
			ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			if ( IsValid(GameMode) )
			{
				GameMode->GetMainHUD()->SetVisibility(ESlateVisibility::Hidden);
			}
			m_SeqPlayer->Play();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("이미 플레이함"));
			return;
		}
	}
}

void AInteraction_Lever::EndLevelSequence()
{
	m_Trigger->DestroyComponent();
	
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		GameMode->GetMainHUD()->SetVisibility(ESlateVisibility::Visible);
		GameMode->GetMainHUD()->ShowMainMessageUI(false);

		const UWorld* World = GetWorld();
		if ( World )
		{
			UFadeViewportClient* GameViewportClient = Cast<UFadeViewportClient>(World->GetGameViewport());
			if ( GameViewportClient )
			{
				GameViewportClient->Fade(1.f, false);
			}
		}
	}
}

//void AInteraction_Lever::FadeInOutWidget(bool _InOut)
//{
//	UE_LOG(LogTemp, Warning, TEXT("faded"));
//	UE_LOG(LogTemp, Warning, TEXT("FadeRate : %f"), fFadeRate);
//	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
//	if ( IsValid(GameMode) )
//	{
//		if (_InOut)
//		{
//			GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this, GameMode]
//			{
//				fFadeRate = FMath::Clamp(fFadeRate - 0.01f, 0.f, 1.f);
//				UE_LOG(LogTemp, Warning, TEXT("FadeRate : %f"), fFadeRate);
//				GameMode->GetMainHUD()->SetRenderOpacity(fFadeRate);
//				if ( fFadeRate <= 0.f )
//				{
//					GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
//				}
//			}
//			, 0.01f, true);
//		}
//		else
//		{
//			GameMode->GetMainHUD()->FadeScreen(1.f);
//			GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this, GameMode]
//			{
//			fFadeRate = FMath::Clamp(fFadeRate + 0.01f, 0.f, 1.f);
//			GameMode->GetMainHUD()->SetRenderOpacity(fFadeRate);
//			if ( fFadeRate >= 1.f )
//			{
//				GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
//			}
//			}
//			, 0.01f, true);
//		}
//	}
//
//}