// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Attack.h"
#include "../Player_Base_Knight.h"

void State_Attack::Enter(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter AttackState"));
	Character->GuardStateOnPlayMontage(true);
	Character->SetbNextAtkCheck(false);
	uint8 Combo = Character->GetCurrentCombo();
	UE_LOG(LogTemp, Warning, TEXT("Combo : %d"), Combo);
	if (++Combo == 1)
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::ATTACK));
	}
	else
	{
		if (Combo > 3)
		{
			Combo = 2;
		}
		UE_LOG(LogTemp, Warning, TEXT("jumpSection"));
		FName NextComboCount = FName(*FString::Printf(TEXT("Combo%d"), Combo));
		Character->GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextComboCount, Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::ATTACK));
	}

	Character->SetCurrentCombo(Combo);
}

void State_Attack::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	// 공격 판정 트레이스
	if ( Character->GetbAtkTrace() )
	{
		Character->AttackHitCheck();
	}
}

void State_Attack::Exit(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit AttackState"));
	Character->SetbAtkTrace(false);
	//Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
}
