// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance_Boss_Base.h"
#include "Anim_GreaterSpider.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRushAttackDelegate, bool);	// 러시공격 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnRangedAttackDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSwingAttackDelegate, EGreaterSpider_STATE);
DECLARE_MULTICAST_DELEGATE(FOnStompAttackDelegate);
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnim_GreaterSpider : public UAnimInstance_Boss_Base
{
	GENERATED_BODY()

private:
	bool bBossAttack;
	bool bIsAtkMove;
	bool bIsTurn;

public:
	FOnRushAttackDelegate	OnRushAttack;
	FOnRangedAttackDelegate OnRangedAttack;
	FOnSwingAttackDelegate OnSwingAttack;
	FOnStompAttackDelegate OnStompAttack;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

	UFUNCTION()
	void AnimNotify_DeadAnimEnd();

	UFUNCTION()
	void AnimNotify_RushStart();

	UFUNCTION()
	void AnimNotify_RushEnd();

	UFUNCTION()
	void AnimNotify_AttackStart();
	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_ShotProj();

	UFUNCTION()
	void AnimNotify_Stomp();

	UFUNCTION()
	void AnimNotify_SwingArm_Left();

	UFUNCTION()
	void AnimNotify_SwingArm_Right();
};
