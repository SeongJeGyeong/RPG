// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Enum.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_MonsterAnim.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_MonsterAnim : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Animation", Meta = ( DisplayName = "MontageData" ))
	TMap<EMON_MONTAGE, UAnimMontage*> m_MonsterAnim;

public:
	UAnimMontage* GetMonsterMontage(EMON_MONTAGE _Montage) const { return *m_MonsterAnim.Find(_Montage); }
};
