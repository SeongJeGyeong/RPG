// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Action.h"
#include "../Player_Base_Knight.h"
#include "../Comp/Player_InputComponent.h"

void State_Action::Enter(APlayer_Base_Knight* Character)
{
}

void State_Action::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->GetInputComp()->SetState(EPlayerStateType::IDLE);
	}
}

void State_Action::Exit(APlayer_Base_Knight* Character)
{
}
