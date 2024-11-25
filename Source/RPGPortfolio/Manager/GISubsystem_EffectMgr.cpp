// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_EffectMgr.h"
#include "../GameInstance_Base.h"
#include "../Header/Enum.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "../System/DataAsset/DA_ItemEffect.h"
#include "../Projectiles/Projectile_Base.h"

void UGISubsystem_EffectMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_HitParticle = FSoftObjectPath("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'");
	m_HitParticle.ToSoftObjectPath().PostLoadPath(nullptr);

	m_ItemEffect = FSoftObjectPath("/Script/RPGPortfolio.DA_ItemEffect'/Game/Blueprint/DataAsset/BPC_DA_ItemEffect.BPC_DA_ItemEffect'");
	m_ItemEffect.ToSoftObjectPath().PostLoadPath(nullptr);

	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	pGameInst->ASyncLoadDataAsset(m_HitParticle.ToSoftObjectPath());
	pGameInst->ASyncLoadDataAsset(m_ItemEffect.ToSoftObjectPath());
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

void UGISubsystem_EffectMgr::SpawnHitEffect(USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, FVector Scale)
{
	UGameplayStatics::SpawnEmitterAttached(GetHitEffect(), AttachToComponent, AttachPointName, Location, Rotation, Scale, EAttachLocation::KeepRelativeOffset, true);
}

void UGISubsystem_EffectMgr::SpawnEffectAtLocation(const UObject* WorldContextObject, UFXSystemAsset* SystemTemplate, FVector Location, FRotator Rotation, FVector Scale, bool bAutoDestroy, bool bAutoActivate, ENCPoolMethod PoolingMethod, bool bPreCullCheck)
{
	if ( ( SystemTemplate )->IsA(UNiagaraSystem::StaticClass()) )
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, Cast<UNiagaraSystem>(SystemTemplate), Location, Rotation, Scale, bAutoDestroy, bAutoActivate, PoolingMethod, bPreCullCheck);
	}
	else if ( ( SystemTemplate )->IsA(UParticleSystem::StaticClass()) )
	{
		UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, Cast<UParticleSystem>(SystemTemplate), Location, Rotation, Scale, bAutoDestroy, ToPSCPoolMethod(PoolingMethod), bAutoActivate);
	}
}

void UGISubsystem_EffectMgr::SpawnEffectAtLocation(const UObject* WorldContextObject, EEffectType _EffectType, FVector Location, FRotator Rotation, FVector Scale, bool bAutoDestroy, bool bAutoActivate, ENCPoolMethod PoolingMethod, bool bPreCullCheck)
{
	UFXSystemAsset* System = m_ItemEffect.IsPending() ? m_ItemEffect.LoadSynchronous()->GetItemEffect(_EffectType) : m_ItemEffect.Get()->GetItemEffect(_EffectType);
	SpawnEffectAtLocation(WorldContextObject, System, Location, Rotation, Scale, bAutoDestroy, bAutoActivate, PoolingMethod, bPreCullCheck);
}

void UGISubsystem_EffectMgr::SpawnEffectAttached(UFXSystemAsset* SystemTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy, FVector Scale, bool bAutoActivate, ENCPoolMethod PoolingMethod, bool bPreCullCheck)
{
	if ( ( SystemTemplate )->IsA(UNiagaraSystem::StaticClass()) )
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(Cast<UNiagaraSystem>(SystemTemplate), AttachToComponent, AttachPointName, Location, Rotation, Scale, LocationType, bAutoDestroy, PoolingMethod, bAutoActivate, bPreCullCheck);
	}
	else if ( ( SystemTemplate )->IsA(UParticleSystem::StaticClass()) )
	{
		UGameplayStatics::SpawnEmitterAttached(Cast<UParticleSystem>(SystemTemplate), AttachToComponent, AttachPointName, Location, Rotation, Scale, LocationType, bAutoDestroy, ToPSCPoolMethod(PoolingMethod), bAutoActivate);
	}
}

void UGISubsystem_EffectMgr::SpawnEffectAttached(EEffectType _EffectType, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy, FVector Scale, bool bAutoActivate, ENCPoolMethod PoolingMethod, bool bPreCullCheck)
{
	UFXSystemAsset* System = m_ItemEffect.IsPending() ? m_ItemEffect.LoadSynchronous()->GetItemEffect(_EffectType) : m_ItemEffect.Get()->GetItemEffect(_EffectType);
	SpawnEffectAttached(System, AttachToComponent, AttachPointName, Location, Rotation, LocationType, bAutoDestroy, Scale, bAutoActivate, PoolingMethod, bPreCullCheck);
}
