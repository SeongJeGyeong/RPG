// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_MonsterInfo.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_MonsterInfo : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = ( DisplayName = "SoundData" ))
	TMap<EMONSTER_TYPE, FMonsterSoundData> m_MonSoundMap;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = ( DisplayName = "AnimMontage" ))
	TMap<EMONSTER_TYPE, FMonsterAnimData> m_MonAnimMap;

public:
	TMap<EMONSTER_TYPE, FMonsterSoundData> GetSoundMap() { return m_MonSoundMap; }
	TMap<EMONSTER_TYPE, FMonsterAnimData> GetAnimMap() { return m_MonAnimMap; }

};
