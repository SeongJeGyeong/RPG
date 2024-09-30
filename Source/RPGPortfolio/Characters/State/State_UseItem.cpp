// Fill out your copyright notice in the Description page of Project Settings.


#include "State_UseItem.h"
#include "../Player_Base_Knight.h"

void State_UseItem::Enter(APlayer_Base_Knight* Character)
{

}

void State_UseItem::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->SetState(EPlayerStateType::IDLE);
	}
}

void State_UseItem::Exit(APlayer_Base_Knight* Character)
{
}
