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
	UPROPERTY()
	EATTACK_TYPE ATK_TYPE;

public:
	EATTACK_TYPE GetAtkType() const { return ATK_TYPE; }
	void SetAtkType(const EATTACK_TYPE& _AtkType) { ATK_TYPE = _AtkType; }

};
