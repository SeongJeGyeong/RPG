// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_SkillComponent.h"
#include "../Projectiles/Proj_Player_Cutter.h"
#include "../System/Subsys_ObjectPool.h"
#include "../Manager/GISubsystem_StatMgr.h"
#include "../System/DataAsset/DA_PlayerSkill.h"

// Sets default values for this component's properties
UPlayer_SkillComponent::UPlayer_SkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static  ConstructorHelpers::FObjectFinder<UDA_PlayerSkill> skillda(TEXT("/Script/RPGPortfolio.DA_PlayerSkill'/Game/Blueprint/DataAsset/BPC_DA_PlayerSkill.BPC_DA_PlayerSkill'"));
	if ( skillda.Succeeded() )
	{
		m_SkillDA = skillda.Object;
	}

	m_SkillName = ESkillName::Slash_Cutter;
}

// Called when the game starts
void UPlayer_SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Skill = m_SkillDA->GetPlayerSkill(m_SkillName);
	if ( m_Skill->Projectile != nullptr )
	{
		if (IsValid(GetOwner()))
		{
			if ( m_SkillName == ESkillName::Slash_Cutter )
			{
				UE_LOG(LogTemp, Warning, TEXT("스킬 풀링"));
				USubsys_ObjectPool* PoolSubsystem = GetOwner()->GetWorld()->GetSubsystem<USubsys_ObjectPool>();
				if ( !PoolSubsystem->PreLoadObjFromPool<AProj_Player_Cutter>(m_Skill->Projectile, 2, GetOwner()) )
				{
					UE_LOG(LogTemp, Error, TEXT("스킬 투사체 풀링 실패"));
				}
			}
		}
	}
}

void UPlayer_SkillComponent::ShotSkillProj(FVector _SpawnLoc, FRotator _SpawnRot, FVector _ShotVec)
{
	USubsys_ObjectPool* ObjectPool = GetOwner()->GetWorld()->GetSubsystem<USubsys_ObjectPool>();
	if (!IsValid(ObjectPool))
	{
		UE_LOG(LogTemp, Error, TEXT("오브젝트 풀 가져오기 실패"));
		return;
	}
	AProj_Player_Cutter* pProjectile = ObjectPool->SpawnObjFromPool<AProj_Player_Cutter>(m_Skill->Projectile, _SpawnLoc, _SpawnRot, GetOwner());
	if ( IsValid(pProjectile) )
	{
		UGISubsystem_StatMgr* StatMgr = GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
		pProjectile->SetProjDamage(EATTACK_TYPE::MAGIC_RANGE, StatMgr->GetPlayerBasePower().MagicAtk);
		pProjectile->LaunchMotion(_ShotVec);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("투사체 로드 실패"));
	}
}