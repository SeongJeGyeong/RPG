// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_EffectMgr.h"
#include "../System/DataAsset/DA_ProjectileAsset.h"
#include "../GameInstance_Base.h"
#include "../Header/Enum.h"

void UGISubsystem_EffectMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	/*m_ProjAsset = FSoftObjectPath("/Script/RPGPortfolio.DA_ProjectileAsset'/Game/Blueprint/DataAsset/BPC_DA_Projectile.BPC_DA_Projectile'");
	m_ProjAsset.ToSoftObjectPath().PostLoadPath(nullptr);*/

	m_HitParticle = FSoftObjectPath("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'");
	m_HitParticle.ToSoftObjectPath().PostLoadPath(nullptr);

	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	pGameInst->ASyncLoadDataAsset(m_HitParticle.ToSoftObjectPath());
}

void UGISubsystem_EffectMgr::Deinitialize()
{
	Super::Deinitialize();
}

UParticleSystem* UGISubsystem_EffectMgr::GetHitEffect() const
{
	if ( m_HitParticle.IsPending() )
	{
		return m_HitParticle.LoadSynchronous();
	}

	return m_HitParticle.Get();
}

//USoundBase* UGISubsystem_EffectMgr::GetProjHitSound(EProjectileType _ProjType) const
//{
//	if ( m_ProjAsset.IsPending() )
//	{
//		return m_ProjAsset.LoadSynchronous()->GetProjectileData(_ProjType).ProjHitSound;
//	}
//	return m_ProjAsset.Get()->GetProjectileData(_ProjType).ProjHitSound;
//}
