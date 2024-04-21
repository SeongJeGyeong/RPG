// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Enum.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerMontage.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_PlayerMontage : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Anim", Meta = ( DisplayName = "MontageData" ))
	TMap<EPlayerMontage, UAnimMontage*> m_MontageMap;

public:
	UAnimMontage* GetPlayerMontage(EPlayerMontage _Montage) { return *m_MontageMap.Find(_Montage); }

};
