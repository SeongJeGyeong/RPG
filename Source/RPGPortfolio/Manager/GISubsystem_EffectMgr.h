// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NiagaraCommon.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_EffectMgr.generated.h"

#define GETHITEFFECT			GetGameInstance()->GetSubsystem<UGISubsystem_EffectMgr>()->GetHitEffect()

enum class EProjectileType : uint8;
enum class EEffectType : uint8;
class UDA_ItemEffect;
class UFXSystemAsset;

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_EffectMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSoftObjectPtr<UParticleSystem>			m_HitParticle;
	
	UPROPERTY()
	TSoftObjectPtr<UDA_ItemEffect>			m_ItemEffect;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UParticleSystem* GetHitEffect() const;

	/* 파티클 시스템일 경우 Pooling Method는 자동 변환 */
	void SpawnEffectAtLocation(const UObject* WorldContextObject, UFXSystemAsset* SystemTemplate, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f), bool bAutoDestroy = true, bool bAutoActivate = true, ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease, bool bPreCullCheck = true);
	void SpawnEffectAtLocation(const UObject* WorldContextObject, EEffectType _EffectType, FVector Location, FRotator Rotation = FRotator::ZeroRotator, FVector Scale = FVector(1.f), bool bAutoDestroy = true, bool bAutoActivate = true, ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease, bool bPreCullCheck = true);

	void SpawnEffectAttached(UFXSystemAsset* SystemTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy, FVector Scale = FVector(1.f), bool bAutoActivate = true, ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease, bool bPreCullCheck = true);
	void SpawnEffectAttached(EEffectType _EffectType, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy, FVector Scale = FVector(1.f), bool bAutoActivate = true, ENCPoolMethod PoolingMethod = ENCPoolMethod::AutoRelease, bool bPreCullCheck = true);

	//USoundBase* GetProjHitSound(EProjectileType _ProjType) const;
};