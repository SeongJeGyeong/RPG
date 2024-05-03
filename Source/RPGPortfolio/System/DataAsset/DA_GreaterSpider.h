// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Enum.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_GreaterSpider.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_GreaterSpider : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = (DisplayName = "AnimMontage"))
	TMap<EGreaterSpider_STATE, TSoftObjectPtr<UAnimMontage>> m_AnimMap;

	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (DisplayName = "SoundBase"))
	TMap<EGreaterSpider_STATE, TSoftObjectPtr<USoundBase>> m_SoundMap;

public:
	TSoftObjectPtr<UAnimMontage> GetAnimGSpider(EGreaterSpider_STATE _State) const { return m_AnimMap.Find(_State)->LoadSynchronous(); }
	TSoftObjectPtr<USoundBase> GetSoundGSpider(EGreaterSpider_STATE _State) const { return m_SoundMap.Find(_State)->LoadSynchronous(); }

};
