// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_EffectMgr.generated.h"

#define GETHITEFFECT			GetGameInstance()->GetSubsystem<UGISubsystem_EffectMgr>()->GetHitEffect()

enum class EProjectileType : uint8;
class UDA_ProjectileAsset;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_EffectMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSoftObjectPtr<UDA_ProjectileAsset>		m_ProjAsset;

	UPROPERTY()
	TSoftObjectPtr<UParticleSystem>			m_HitParticle;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UParticleSystem* GetHitEffect() const;
	//USoundBase* GetProjHitSound(EProjectileType _ProjType) const;
};
