// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageType_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDamageType_Base : public UDamageType
{
	GENERATED_BODY()

private:
	EATTACK_TYPE	Atk_Type;
	EATTACK_WEIGHT	Atk_Weight;
public:
	EATTACK_TYPE GetAtkType() const { return Atk_Type; }
	void SetAtkType(const EATTACK_TYPE& _AtkType) { Atk_Type = _AtkType; }

	EATTACK_WEIGHT GetAtkWeight() const { return Atk_Weight; }
	void SetAtkWeight(const EATTACK_WEIGHT& _AtkWeight) { Atk_Weight = _AtkWeight; }
};
