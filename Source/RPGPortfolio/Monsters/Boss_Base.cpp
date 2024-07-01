// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "AI/Boss/AIC_Boss_Base.h"
#include "Components/WidgetComponent.h"
#include "../System/DamageType_Base.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Player_Base_Knight.h"
#include "Components/CapsuleComponent.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_Boss.h"
// Sets default values
ABoss_Base::ABoss_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIC_Boss_Base::StaticClass();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCapGroup"));
	GetMesh()->SetCollisionProfileName(TEXT("MonsterMeshGroup"));
}

void ABoss_Base::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);

	FMonsterInfo* pInfo;

	if ( IsValid(m_MonsterInfoTableRow.DataTable) && !m_MonsterInfoTableRow.RowName.IsNone() )
	{
		pInfo = m_MonsterInfoTableRow.DataTable->FindRow<FMonsterInfo>(m_MonsterInfoTableRow.RowName, TEXT(""));

		if (nullptr != pInfo)
		{
			m_Info = *pInfo;
		}
	}
}

// Called when the game starts or when spawned
void ABoss_Base::BeginPlay()
{
	Super::BeginPlay();

	AAIController* pAIController = Cast<AAIController>(GetController());
	if (IsValid(pAIController))
	{
		// 블랙보드에 몬스터정보 전달
		if ( pAIController->GetBlackboardComponent() )
		{
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AtkRange"), m_Info.AtkRange);
			ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (!IsValid(pPlayer))
			{
				return;
			}
			pAIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), pPlayer);
			pAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("bStop"), true);
		}
	}

	ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( !IsValid(pGameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
		return;
	}
	UUI_Base* pMainUI = pGameMode->GetMainHUD();
	m_BossWidget = pMainUI->GetBossUI();
	m_BossWidget->SetName(m_Info.Name);
	m_BossWidget->SetHPRatio(1.f);
	m_BossWidget->SetVisibility(ESlateVisibility::Visible);
}

// Called every frame
void ABoss_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss_Base::MonsterDead()
{
	bIsDead = true;

	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		GameMode->PlayBGM(false);
	}

	USoundBase* pSound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Blueprint/Monster/Sound/SC_Boss_Defeat.SC_Boss_Defeat'"));
	UGameplayStatics::PlaySound2D(GetWorld(), pSound);
	// 5초 뒤 사망 이펙트 처리
	GetWorld()->GetTimerManager().SetTimer(DeadTimer, [this]()
	{
		fDeadEffectRatio += 0.005f;

		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("EffectRatio"), fDeadEffectRatio);

		if (fDeadEffectRatio > 0.7f)
		{
			GetWorld()->GetTimerManager().ClearTimer(DeadTimer);
			m_BossWidget->SetVisibility(ESlateVisibility::Hidden);
			Destroy();
		}
	},
	0.01f, true, 5.f);
}

void ABoss_Base::StopBoneHitReaction(FName _BoneName)
{
	fPhysicsWeight -= GetWorld()->GetDeltaSeconds() * 2.f;
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(_BoneName, fPhysicsWeight);
	if (fPhysicsWeight <= 0.f)
	{
		GetMesh()->SetAllBodiesBelowSimulatePhysics(_BoneName, false);
		GetWorld()->GetTimerManager().ClearTimer(HitReactTimer);
		HitReactDelegate.Unbind();
	}
}

void ABoss_Base::SetBossLockedOn(const bool& _LockedOn)
{
	GetMesh()->SetRenderCustomDepth(_LockedOn);
}