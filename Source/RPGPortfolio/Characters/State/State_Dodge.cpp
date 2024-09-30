// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Dodge.h"
#include "../Player_Base_Knight.h"
#include "../../System/DataAsset/DA_PlayerMontage.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../CharacterAnim/AnimInstance_Knight.h"

void State_Dodge::Enter(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter DodgeState"));

	UAnimInstance_Knight* pAnimInst = Cast<UAnimInstance_Knight>(Character->GetMesh()->GetAnimInstance());
	if ( pAnimInst == nullptr )
	{
		UE_LOG(LogTemp, Error, TEXT("애님 인스턴스 가져오기 실패"));
		return;
	}

	if ( Character->GetCharacterMovement()->GetLastInputVector().IsZero() )
	{
		pAnimInst->Montage_Play(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::DODGE_BW));
		DodgeType = EPlayerMontage::DODGE_BW;
	}
	else
	{
		pAnimInst->Montage_Play(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::DODGE_FW));
		DodgeType = EPlayerMontage::DODGE_FW;
		Character->MotionWarping_Dodge(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::DODGE_FW), 0.1f);
	}

	Character->SetCurrentCombo(1);
}

void State_Dodge::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	// 회피 애니메이션 재생중일 때
	//Character->SetActorRotation(rDodgeRotation);
}

void State_Dodge::Exit(APlayer_Base_Knight* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit DodgeState"));

	vDodgeVector = FVector::ZeroVector;
	rDodgeRotation = FRotator::ZeroRotator;
	Character->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
	Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}
