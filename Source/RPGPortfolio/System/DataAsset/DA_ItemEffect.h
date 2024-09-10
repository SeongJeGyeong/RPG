// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Enum.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ItemEffect.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_ItemEffect : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Effect", Meta = ( DisplayName = "EffectData" ))
	TMap<EEffectType, class UFXSystemAsset*> m_ItemEffectMap;

public:
	UFXSystemAsset* GetItemEffect(EEffectType _EffectType) const { return *m_ItemEffectMap.Find(_EffectType); }

};
