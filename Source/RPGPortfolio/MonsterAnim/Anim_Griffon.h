// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance_Boss_Base.h"
#include "Anim_Griffon.generated.h"

/**
 *	사용안함
 */
UCLASS()
class RPGPORTFOLIO_API UAnim_Griffon : public UAnimInstance_Boss_Base
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool bIsFly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	class AMonster_Griffon* m_Monster;

public:
	//UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info")
	//EBOSS_STATE m_State;

	bool bBossAttack;
	bool bIsAtkMove;
	bool bIsTurn;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;
	void MeleeAttack(EBOSS_STATE _BState);
	void PlayTurnMontage(int32 _Dir);

	UFUNCTION()
	void AnimNotify_BossAtkEnd() { bBossAttack = false; }
	UFUNCTION()
	void AnimNotify_BossTurnEnd() { bIsTurn = false; }
	UFUNCTION()
	void AnimNotify_MoveStart() { bIsAtkMove = true; }
	UFUNCTION()
	void AnimNotify_MoveEnd() { bIsAtkMove = false; }

	UFUNCTION()
	void AnimNotify_HitCheckStart();
	UFUNCTION()
	void AnimNotify_HitCheckEnd();
	UFUNCTION()
	void AnimNotify_Combo1();
	UFUNCTION()
	void AnimNotify_Combo2();
	UFUNCTION()
	void AnimNotify_Combo3();

};
