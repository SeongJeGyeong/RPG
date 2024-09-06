// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PDA_MonsterAssets.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPDA_MonsterAssets : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = ( DisplayName = "AnimMontage" ))
	TMap<EMONSTER_TYPE, FMonsterAnimAsset> m_MonsterAnim;

public:
	FMonsterAnimAsset* GetMonAnimData(EMONSTER_TYPE _MonType) { return m_MonsterAnim.Find(_MonType); }

};
