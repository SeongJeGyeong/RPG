// Fill out your copyright notice in the Description page of Project Settings.


#include "State_AttackWait.h"
#include "../Player_Base_Knight.h"

void State_AttackWait::Enter(APlayer_Base_Knight* Character)
{
}

void State_AttackWait::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( Character->GetbIsAttacking() )
	{
		Character->AttackStart();
		return;
	}

	if ( !Character->GetLastMovementInputVector().IsZero() )
	{
		Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
		Character->SetState(EPlayerStateType::IDLE);
	}
}

void State_AttackWait::Exit(APlayer_Base_Knight* Character)
{
	Character->SetbEnableComboInput(false);
	Character->SetbIsAttacking(false);
}
