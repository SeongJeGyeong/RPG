// Fill out your copyright notice in the Description page of Project Settings.


#include "State_HeavyAttack.h"
#include "../Player_Base_Knight.h"

void State_HeavyAttack::Enter(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter HeavyAttackState"));
	Character->GuardStateOnPlayMontage(true);
	Character->SetbNextAtkCheck(false);
	uint8 Combo = Character->GetCurrentCombo();
	if (++Combo == 1)
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Play(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK));
	}
	else
	{
		if (Combo > 2)
		{
			Combo = 1;
		}
		UE_LOG(LogTemp, Warning, TEXT("jumpSection"));
		FName NextComboCount = FName(*FString::Printf(TEXT("Combo%d"), Combo));
		Character->GetMesh()->GetAnimInstance()->Montage_JumpToSection(NextComboCount, Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK));
	}

	Character->SetCurrentCombo(Combo);
}

void State_HeavyAttack::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	// 공격 판정 트레이스
	if ( Character->GetbAtkTrace() )
	{
		Character->AttackHitCheck();
	}
}

void State_HeavyAttack::Exit(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit HeavyAttackState"));
	Character->SetbAtkTrace(false);
	//Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
}
