// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_SkillComponent.h"
#include "../Projectiles/Proj_Player_Cutter.h"
#include "../System/Subsys_ObjectPool.h"
#include "../Manager/GISubsystem_StatMgr.h"

// Sets default values for this component's properties
UPlayer_SkillComponent::UPlayer_SkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FClassFinder<AProj_Player_Cutter> projectile(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Projectile/BPC_Proj_Cutter.BPC_Proj_Cutter_C'"));
	if ( projectile.Succeeded() )
	{
		ProjSubclass = projectile.Class;
	}
}

// Called when the game starts
void UPlayer_SkillComponent::BeginPlay()
{
	Super::BeginPlay();

	if ( ProjSubclass != nullptr )
	{
		if (IsValid(GetOwner()))
		{
			USubsys_ObjectPool* PoolSubsystem = GetOwner()->GetWorld()->GetSubsystem<USubsys_ObjectPool>();
			if ( !PoolSubsystem->PreLoadObjFromPool<AProj_Player_Cutter>(ProjSubclass, 2, GetOwner()) )
			{
				UE_LOG(LogTemp, Error, TEXT("스킬 투사체 풀링 실패"));
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
	AProj_Player_Cutter* pProjectile = ObjectPool->SpawnObjFromPool<AProj_Player_Cutter>(ProjSubclass, _SpawnLoc, _SpawnRot, GetOwner());
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