// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Hit.h"
#include "../Player_Base_Knight.h"

void State_Hit::Enter(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter HitState"));
	Character->GuardStateOnPlayMontage(true);
}

void State_Hit::Update(APlayer_Base_Knight* Character, float DeltaTime)
{

}

void State_Hit::Exit(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit HitState"));
	//Character->GuardStateOnPlayMontage(false);
}
