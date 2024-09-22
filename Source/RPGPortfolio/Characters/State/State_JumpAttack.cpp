// Fill out your copyright notice in the Description page of Project Settings.


#include "State_JumpAttack.h"
#include "../Player_Base_Knight.h"

void State_JumpAttack::Enter(APlayer_Base_Knight* Character)
{
	Character->GetMesh()->GetAnimInstance()->Montage_Play(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::JUMPATTACK));
	Character->SetbInvalidInput(true);
	Character->SetCurrentCombo(0);
}

void State_JumpAttack::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
}

void State_JumpAttack::Exit(APlayer_Base_Knight* Character)
{
}
