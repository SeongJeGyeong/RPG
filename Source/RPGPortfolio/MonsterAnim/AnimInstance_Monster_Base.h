// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Monster_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstance_Monster_Base : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	class AMonster_Base* m_Monster;
	TSoftObjectPtr<UAnimMontage> m_HitMontage;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	EMONSTER_STATE m_State;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

public:
	void SetDeadAnim() { bIsDead = true; }

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

	UFUNCTION()
	void AnimNotify_HitCheckStart();

	UFUNCTION()
	void AnimNotify_HitCheckEnd();

	UFUNCTION()
	void AnimNotify_Hit_Start();

	UFUNCTION()
	void AnimNotify_Hit_End();
};
