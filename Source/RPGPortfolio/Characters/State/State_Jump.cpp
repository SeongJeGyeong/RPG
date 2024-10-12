// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Jump.h"
#include "../Player_Base_Knight.h"

void State_Jump::Enter(APlayer_Base_Knight* Character)
{
	Character->SetCurrentCombo(1);
	Character->Jump();
}

void State_Jump::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
}

void State_Jump::Exit(APlayer_Base_Knight* Character)
{
}
