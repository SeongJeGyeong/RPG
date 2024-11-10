// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Attack.h"
#include "../Player_Base_Knight.h"


void State_Attack::Enter(APlayer_Base_Knight* Character)
{
	Character->SetbEnableAtkInput(false);
	Character->SetbIsAttacking(false);
	uint8 Combo = Character->GetCurrentCombo();

	EPlayerMontage MontageType = EPlayerMontage::ATTACK_1;
	if ( Combo > 3 )
	{
		Combo = 1;
	}
	switch (Combo)
	{
	case 1:
		MontageType = EPlayerMontage::ATTACK_1;
		break;
	case 2:
		MontageType = EPlayerMontage::ATTACK_2;
		break;
	case 3:
		MontageType = EPlayerMontage::ATTACK_3;
		break;
	default:
		break;
	}
	Character->SetCurrentCombo(++Combo);
	Character->PlayerMotionWarping(MontageType, 0.3f);
	Character->Play_PlayerMontage(MontageType);
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
	Character->SetAttackTrace(false);
}
