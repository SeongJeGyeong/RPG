// Fill out your copyright notice in the Description page of Project Settings.


#include "State_HeavyAttack.h"
#include "../Player_Base_Knight.h"

void State_HeavyAttack::Enter(APlayer_Base_Knight* Character)
{
	Character->SetbEnableAtkInput(false);
	Character->SetbIsAttacking(false);
	Character->GetCharacterMovement()->MaxWalkSpeed = 0.f;
	uint8 Combo = Character->GetCurrentCombo();

	if ( Combo > 3 )
	{
		Combo = 1;
	}

	EPlayerMontage MontageType = EPlayerMontage::HEAVYATTACK_1;
	switch ( Combo )
	{
	case 1:
		MontageType = EPlayerMontage::HEAVYATTACK_1;
		break;
	case 2:
		MontageType = EPlayerMontage::HEAVYATTACK_2;
		break;
	case 3:
		MontageType = EPlayerMontage::HEAVYATTACK_3;
		break;
	default:
		break;
	}

	Character->SetCurrentCombo(++Combo);
	Character->PlayerMotionWarping(MontageType, 0.45f);
	Character->Play_PlayerMontage(MontageType);
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
	Character->SetbAtkTrace(false);
}
