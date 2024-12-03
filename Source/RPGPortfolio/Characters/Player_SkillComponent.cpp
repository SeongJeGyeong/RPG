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

	static ConstructorHelpers::FObjectFinder<UDA_PlayerSkill> skillda(TEXT("/Script/RPGPortfolio.DA_PlayerSkill'/Game/Blueprint/DataAsset/BPC_DA_PlayerSkill.BPC_DA_PlayerSkill'"));
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

	if ( !m_SkillDA || !m_SkillDA->IsValidLowLevel() )
	{
		UE_LOG(LogTemp, Error, TEXT("m_SkillDA 로드 실패"));
		return;
	}
	else
	{
		m_Skill = m_SkillDA->GetPlayerSkill(m_SkillName);
	}

	if ( m_Skill->Projectile != nullptr )
	{
		if (IsValid(GetOwner()))
		{
			if ( m_SkillName == ESkillName::Slash_Cutter )
			{
				UE_LOG(LogTemp, Warning, TEXT("스킬 풀링"));
				USubsys_ObjectPool* PoolSubsystem = GetOwner()->GetWorld()->GetSubsystem<USubsys_ObjectPool>();
				if ( !IsValid(PoolSubsystem) || !PoolSubsystem->PreLoadObjFromPool<AProj_Player_Cutter>(m_Skill->Projectile, 2, GetOwner()) )
				{
					UE_LOG(LogTemp, Error, TEXT("스킬 투사체 풀링 실패"));
				}
			}
		}
	}
}

void UPlayer_SkillComponent::ShotProjectile()
{
	USubsys_ObjectPool* ObjectPool = GetOwner()->GetWorld()->GetSubsystem<USubsys_ObjectPool>();
	if ( !IsValid(ObjectPool) )
	{
		UE_LOG(LogTemp, Error, TEXT("오브젝트 풀 가져오기 실패"));
		return;
	}

	// 투사체 생성위치	
	FVector ProjectileLocation = GetOwner()->GetActorLocation() + FVector(0.f, 0.f, 10.f) + GetOwner()->GetActorForwardVector() * 200.f;
	AProj_Player_Cutter* pProjectile = ObjectPool->SpawnObjFromPool<AProj_Player_Cutter>(m_Skill->Projectile, ProjectileLocation, GetOwner()->GetActorRotation(), GetOwner());
	if ( IsValid(pProjectile) )
	{
		UGISubsystem_StatMgr* StatMgr = GetOwner()->GetWorld()->GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
		if ( IsValid(StatMgr) ) pProjectile->SetProjDamage(EATTACK_TYPE::MAGIC_RANGE, StatMgr->GetPlayerBasePower().MagicAtk);

		// 투사체 발사 방향
		FVector vDir = GetOwner()->GetActorForwardVector() * 1000.f;

		pProjectile->LaunchMotion(vDir);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("투사체 로드 실패"));
	}
}
