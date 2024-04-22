// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Enum.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerSound.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_PlayerSound : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = ( DisplayName = "SoundData" ))
	TMap<EPlayerSound, USoundBase*> m_PlayerSoundMap;

public:
	USoundBase* GetPlayerSound(EPlayerSound _SoundType) const { return *m_PlayerSoundMap.Find(_SoundType); }

};
