// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StateMachine.h"
#include "CoreMinimal.h"

/**
 * 
 */
class RPGPORTFOLIO_API State_Dodge : public StateMachine
{
private:
	FVector vDodgeVector;
	FRotator rDodgeRotation;
	EPlayerMontage DodgeType;
public:
	virtual void Enter(APlayer_Base_Knight* Character) override;
	virtual void Update(APlayer_Base_Knight* Character, float DeltaTime) override;
	virtual void Exit(APlayer_Base_Knight* Character) override;
	virtual EPlayerStateType GetStateType() override { return EPlayerStateType::DODGE; }
};
