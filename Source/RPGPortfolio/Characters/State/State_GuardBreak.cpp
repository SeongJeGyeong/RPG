// Fill out your copyright notice in the Description page of Project Settings.


#include "State_GuardBreak.h"
#include "../Player_Base_Knight.h"

void State_GuardBreak::Enter(APlayer_Base_Knight* Character)
{
	Character->Play_PlayerMontage(EPlayerMontage::GUARDBREAK);
	Character->SetbHoldGuard(false);
	Character->SetfGuardWeight(0.f);
	Character->SetbInputGuard(false);
}

void State_GuardBreak::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->SetState(EPlayerStateType::IDLE);
	}
}

void State_GuardBreak::Exit(APlayer_Base_Knight* Character)
{
}
