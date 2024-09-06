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
	TMap<EGreaterSpider_STATE, UAnimMontage*> m_Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Sound", Meta = (DisplayName = "SoundBase"))
	TMap<EGreaterSpider_STATE, USoundBase*> m_Sound;

public:
	UAnimMontage* GetAnimGSpider(EGreaterSpider_STATE _State) const { return *m_Anim.Find(_State); }
	USoundBase* GetSoundGSpider(EGreaterSpider_STATE _State) const { return *m_Sound.Find(_State); }

};
