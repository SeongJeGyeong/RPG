// Fill out your copyright notice in the Description page of Project Settings.


#include "State_UseSkill.h"
#include "../Player_Base_Knight.h"

void State_UseSkill::Enter(APlayer_Base_Knight* Character)
{
}

void State_UseSkill::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->SetState(EPlayerStateType::IDLE);
	}
}

void State_UseSkill::Exit(APlayer_Base_Knight* Character)
{
}
