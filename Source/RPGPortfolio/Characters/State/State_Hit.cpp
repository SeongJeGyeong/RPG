// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Hit.h"
#include "../Player_Base_Knight.h"
#include "../Comp/Player_InputComponent.h"

void State_Hit::Enter(APlayer_Base_Knight* Character)
{
	Character->ResetVarsOnHitState();
}

void State_Hit::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->GetInputComp()->SetState(EPlayerStateType::IDLE);
		return;
	}

	if ( Character->GetMesh()->GetAnimInstance()->Montage_IsPlaying(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::HIT_AIR)) &&
		!Character->GetCharacterMovement()->IsFalling() )
	{
		if ( !FName("Land").IsEqual(Character->GetMesh()->GetAnimInstance()->Montage_GetCurrentSection()) )
		{
			Character->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Land"), Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::HIT_AIR));
		}
	}
}

void State_Hit::Exit(APlayer_Base_Knight* Character)
{
}
