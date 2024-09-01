// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Monster_Base.generated.h"

DECLARE_MULTICAST_DELEGATE(FHitEndDelegate); // 피격 몽타주 종료 델리게이트
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	bool bIsMove;	// Idle/Move 전환용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	FVector	vLocalVelocity;	// 이동 블렌드 스페이스용 벡터

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = ( AllowPrivateAccess = "true" ))
	class UCharacterMovementComponent* m_Movement;

public:
	void SetDeadAnim() { bIsDead = true; }

public:
	FHitEndDelegate OnHitEnd;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

	UFUNCTION()
	void AnimNotify_HitCheckStart();

	UFUNCTION()
	void AnimNotify_HitCheckEnd();

	UFUNCTION()
	void AnimNotify_NextAtkCheck();

	UFUNCTION()
	void AnimNotify_HitEnd();
};
