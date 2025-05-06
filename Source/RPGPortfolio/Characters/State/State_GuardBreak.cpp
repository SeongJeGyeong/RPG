// Fill out your copyright notice in the Description page of Project Settings.


#include "State_GuardBreak.h"
#include "../Player_Base_Knight.h"
#include "../Comp/Player_InputComponent.h"

void State_GuardBreak::Enter(APlayer_Base_Knight* Character)
{
	Character->Play_PlayerMontage(EPlayerMontage::GUARDBREAK);
	Character->SetbHoldGuard(false);
	Character->GetInputComp()->SetbInputGuard(false);
	Character->GetInputComp()->SetGuardWeight(0.f);
}

void State_GuardBreak::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->GetInputComp()->SetState(EPlayerStateType::IDLE);
	}
}

void State_GuardBreak::Exit(APlayer_Base_Knight* Character)
{
}
