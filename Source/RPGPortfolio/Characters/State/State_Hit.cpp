// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Hit.h"
#include "../Player_Base_Knight.h"

void State_Hit::Enter(APlayer_Base_Knight* Character)
{
	if ( Character->GetbHoldGuard() )
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::GUARDBREAK));
		Character->SetbHoldGuard(false);
		Character->SetfGuardWeight(0.f);
	}

}

void State_Hit::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->SetState(EPlayerStateType::IDLE);
	}
}

void State_Hit::Exit(APlayer_Base_Knight* Character)
{
}
