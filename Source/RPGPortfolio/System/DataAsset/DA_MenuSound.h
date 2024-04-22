// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Enum.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_MenuSound.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_MenuSound : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = ( DisplayName = "SoundData" ))
	TMap<EMenuSound, USoundBase*> m_MenuSoundMap;

public:
	USoundBase* GetMenuSound(EMenuSound _SoundType) const { return *m_MenuSoundMap.Find(_SoundType); }

};
