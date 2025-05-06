// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Guard.h"
#include "../Player_Base_Knight.h"
#include "../Comp/Player_InputComponent.h"

void State_Guard::Enter(APlayer_Base_Knight* Character)
{
	Character->Play_PlayerSound(EPlayerSound::GUARDBLOCK);
}

void State_Guard::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->GetInputComp()->SetState(EPlayerStateType::IDLE);
	}
}

void State_Guard::Exit(APlayer_Base_Knight* Character)
{
}
