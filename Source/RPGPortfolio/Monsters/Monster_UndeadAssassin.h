// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster_Base.h"
#include "Monster_UndeadAssassin.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API AMonster_UndeadAssassin : public AMonster_Base
{
	GENERATED_BODY()
	
public:
	AMonster_UndeadAssassin();


public:
	virtual void Tick(float DeltaTime) override;

	void MonsterAttackMelee();
private:
	void MeleeAttackHitCheck();
};
