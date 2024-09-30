// Fill out your copyright notice in the Description page of Project Settings.


#include "State_HeavyAttack.h"
#include "../Player_Base_Knight.h"

void State_HeavyAttack::Enter(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter HeavyAttackState"));

	Character->SetbNextAtkCheck(false);
	Character->SetbIsAttacking(false);
	Character->SetbNextAtkStart(false);
	Character->GetCharacterMovement()->MaxWalkSpeed = 0.f;
	uint8 Combo = Character->GetCurrentCombo();
	UE_LOG(LogTemp, Warning, TEXT("Combo : %d"), Combo);

	if ( Combo > 3 )
	{
		Combo = 1;
	}
	UAnimMontage* Animation = nullptr;
	switch ( Combo )
	{
	case 1:
		Animation = Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK_1);
		break;
	case 2:
		Animation = Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK_2);
		break;
	case 3:
		Animation = Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::HEAVYATTACK_3);
		break;
	default:
		break;
	}

	Character->SetCurrentCombo(++Combo);
	Character->MotionWarping_Attack(Animation, 0.45f);
	Character->GetMesh()->GetAnimInstance()->Montage_Play(Animation);
}

void State_HeavyAttack::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	// 공격 판정 트레이스
	/*if ( Character->GetbAtkTrace() )
	{
		Character->AttackHitCheck();
	}*/

	if (Character->GetbIsAttacking() && Character->GetbNextAtkStart())
	{
		Character->AttackStart();
	}
}

void State_HeavyAttack::Exit(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit HeavyAttackState"));
	//Character->SetbAtkTrace(false);
	Character->SetbNextAtkCheck(false);
	Character->SetbNextAtkStart(false);
	Character->SetbIsAttacking(false);
}
