// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Enum.h"
#include "CoreMinimal.h"

class APlayer_Base_Knight;
/**
 * 
 */
class RPGPORTFOLIO_API StateMachine
{
public:
	virtual void Enter(APlayer_Base_Knight* Character) = 0;
	virtual void Update(APlayer_Base_Knight* Character, float DeltaTime) = 0;
	virtual void Exit(APlayer_Base_Knight* Character) = 0;
	virtual EPlayerStateType GetStateType() = 0;
};
