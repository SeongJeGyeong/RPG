// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Idle.h"
#include "../Player_Base_Knight.h"

void State_Idle::Enter(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter IdleState"));
	Character->GuardStateOnPlayMontage(false);
	Character->SetCurrentCombo(0);
	Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void State_Idle::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Update IdleState"));
}

void State_Idle::Exit(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit IdleState"));
}
