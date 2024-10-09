// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Dodge.h"
#include "../Player_Base_Knight.h"

void State_Dodge::Enter(APlayer_Base_Knight* Character)
{
	if ( Character->GetCharacterMovement()->GetLastInputVector().IsZero() )
	{
		Character->Play_PlayerMontage(EPlayerMontage::DODGE_BW);
	}
	else
	{
		Character->PlayerMotionWarping(EPlayerMontage::DODGE_FW, 0.1f);
		Character->Play_PlayerMontage(EPlayerMontage::DODGE_FW);
	}

	Character->SetCurrentCombo(1);
}

void State_Dodge::Update(APlayer_Base_Knight* Character, float DeltaTime)
{

}

void State_Dodge::Exit(APlayer_Base_Knight* Character)
{
	Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
	Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}
