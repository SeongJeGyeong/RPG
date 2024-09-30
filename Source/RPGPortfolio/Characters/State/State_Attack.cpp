// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Attack.h"
#include "../Player_Base_Knight.h"


void State_Attack::Enter(APlayer_Base_Knight* Character)
{
	Character->SetbNextAtkCheck(false);
	Character->SetbIsAttacking(false);
	Character->SetbNextAtkStart(false);
	Character->GetCharacterMovement()->MaxWalkSpeed = 0.f;
	uint8 Combo = Character->GetCurrentCombo();

	UAnimMontage* Animation = nullptr;
	if ( Combo > 3 )
	{
		Combo = 1;
	}
	switch (Combo)
	{
	case 1:
		Animation = Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::ATTACK_1);
		break;
	case 2:
		Animation = Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::ATTACK_2);
		break;
	case 3:
		Animation = Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::ATTACK_3);
		break;
	default:
		break;
	}
	Character->SetCurrentCombo(++Combo);
	Character->MotionWarping_Attack(Animation, 0.3f);
	Character->GetMesh()->GetAnimInstance()->Montage_Play(Animation);
}

void State_Attack::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	// 공격 판정 트레이스
	if ( Character->GetbAtkTrace() )
	{
		UE_LOG(LogTemp, Warning, TEXT("HitCheck"));
		Character->AttackHitCheck();
	}

	if (Character->GetbIsAttacking() && Character->GetbNextAtkStart())
	{
		Character->AttackStart();
	}
}

void State_Attack::Exit(APlayer_Base_Knight* Character)
{
	//UE_LOG(LogTemp, Warning, TEXT("Exit AttackState"));
	//Character->SetbAtkTrace(false);
	Character->SetbNextAtkCheck(false);
	Character->SetbNextAtkStart(false);
	Character->SetbIsAttacking(false);
	//Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
}
