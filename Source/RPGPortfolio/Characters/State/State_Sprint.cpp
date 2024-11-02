// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Sprint.h"
#include "../Player_Base_Knight.h"
#include "../Player_StatComponent.h"

void State_Sprint::Enter(APlayer_Base_Knight* Character)
{
	Character->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void State_Sprint::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( Character->GetCharacterMovement()->Velocity.Size2D() <= 0.f || Character->GetCharacterMovement()->GetCurrentAcceleration().IsZero() )
	{
		Character->SetState(EPlayerStateType::IDLE);
		return;
	}

	Character->GetStatComp()->DecreasePlayerStamina(10.f * DeltaTime);
	if ( Character->GetStatComp()->IsStaminaZero())
	{
		Character->SetState(EPlayerStateType::IDLE);
		return;
	}
}

void State_Sprint::Exit(APlayer_Base_Knight* Character)
{
	Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}
