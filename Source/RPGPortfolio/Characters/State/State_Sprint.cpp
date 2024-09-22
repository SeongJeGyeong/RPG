// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Sprint.h"
#include "../Player_Base_Knight.h"
#include "../../CharacterAnim/AnimInstance_Knight.h"

void State_Sprint::Enter(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter SprintState"));
	Character->GuardStateOnPlayMontage(true);
	Character->SetbSprintToggle(true);
	Character->GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void State_Sprint::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( Character->GetCharacterMovement()->Velocity.Size2D() <= 0.f || Character->GetCharacterMovement()->GetCurrentAcceleration().IsZero() )
	{
		Character->SetState(EPlayerStateType::IDLE);
		return;
	}
	if (!Character->ConsumeStamina(10.f * DeltaTime))
	{
		Character->SetState(EPlayerStateType::IDLE);
		return;
	}
}

void State_Sprint::Exit(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit SprintState"));
	Character->SetbSprintToggle(false);
	Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}
