// Fill out your copyright notice in the Description page of Project Settings.


#include "State_JumpAttack.h"
#include "../Player_Base_Knight.h"

void State_JumpAttack::Enter(APlayer_Base_Knight* Character)
{
	Character->Play_PlayerMontage(EPlayerMontage::JUMPATTACK);
	Character->SetCurrentCombo(2);
}

void State_JumpAttack::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( Character->GetbAtkTrace() )
	{
		Character->AttackHitCheck();
	}
}

void State_JumpAttack::Exit(APlayer_Base_Knight* Character)
{
	Character->SetCurrentCombo(1);
}
