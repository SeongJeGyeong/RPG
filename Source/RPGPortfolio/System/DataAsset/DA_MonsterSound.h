// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_MonsterSound.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_MonsterSound : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (DisplayName = "SoundData"))
	TMap<EMONSTER_TYPE, FMonsterSoundData> m_MonSoundMap;

public:
	TMap<EMONSTER_TYPE, FMonsterSoundData> GetSoundMap() { return m_MonSoundMap; }
};
