// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Idle.h"
#include "../Player_Base_Knight.h"
#include "../../CharacterAnim/AnimInstance_Knight.h"

void State_Idle::Enter(APlayer_Base_Knight* Character)
{
	Character->SetCurrentCombo(1);
	Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void State_Idle::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	/*if ( Character->GetbInputGuard())
	{
		Character->SetfGuardWeight(FMath::Clamp(Character->GetfGuardWeight() + DeltaTime * 10.f, 0.f, 1.f));
		if ( Character->GetfGuardWeight() >= 1.f )
		{
			Character->SetbHoldGuard(true);
		}
	}
	else
	{
		Character->SetfGuardWeight(FMath::Clamp(Character->GetfGuardWeight() - DeltaTime * 15.f, 0.f, 1.f));
		Character->SetbHoldGuard(false);
	}*/

	if ( Character->GetfGuardWeight() >= 1.f )
	{
		Character->SetbHoldGuard(true);
	}
	else
	{
		Character->SetbHoldGuard(false);
	}
}

void State_Idle::Exit(APlayer_Base_Knight* Character)
{
	Character->SetfGuardWeight(0.f);
	Character->SetbHoldGuard(false);
}
