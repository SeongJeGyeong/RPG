// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_MonAssetMgr.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_MonAssetMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSoftObjectPtr<class UPDA_MonsterAssets>	m_MonsterDataAsset;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FMonsterAnimAsset GetMonAnimAsset(EMONSTER_TYPE _Type);

};
