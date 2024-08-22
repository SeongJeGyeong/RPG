// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Characters/Player_Base_Knight.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Knight.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);					// 다음콤보 체크 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam (FOnDodgeTimeCheckDelegate, bool);	// 무적시간 체크 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnAttackMoveDelegate);						// 공격 중 이동 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnJumpAtkDelegate);							// 점프공격 델리게이트

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstance_Knight : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	APlayer_Base_Knight* m_Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* m_Movement;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float fMoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool bIsMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FVector	vLocalVelocity;	// 이동 블렌드 스페이스용 벡터

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	bool bIsGuard;			// 가드액션 여부

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float fGuardBlendWeight;// 가드 애니메이션 블렌드용 수치

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	bool bIsSprint;			// 달리기 토글 여부

public:
	bool GetbIsGuard() { return bIsGuard; }
	void SetbIsGuard(bool _IsGuard) { bIsGuard = _IsGuard; }

	bool GetbIsSprint() { return bIsSprint; }
	void SetbIsSprint(bool _IsSprint) { bIsSprint = _IsSprint; }
	void SetLocalVelocityXY(FVector2D _Velocity) { vLocalVelocity.X = _Velocity.X; vLocalVelocity.Y = _Velocity.Y; }

public:
	FOnNextAttackCheckDelegate	OnNextAttackCheck;
	FOnDodgeTimeCheckDelegate	OnDodgeTimeCheck;
	FOnAttackMoveDelegate		OnAttackMove;
	FOnJumpAtkDelegate			OnJumpAtk;

	/*UFUNCTION()
	void AnimNotify_AtkSectionStart();*/

	// 다음콤보 체크
	UFUNCTION()
	void AnimNotify_NextCheckStart();
	UFUNCTION()
	void AnimNotify_NextCheckEnd();

	UFUNCTION()
	void AnimNotify_HitCheckStart();
	UFUNCTION()
	void AnimNotify_HitCheckEnd();

	UFUNCTION()
	void AnimNotify_MoveStart();

	UFUNCTION()
	void AnimNotify_DodgeStart();
	UFUNCTION()
	void AnimNotify_DodgeEnd();
	UFUNCTION()
	void AnimNotify_DodgeAnimEnd();

	UFUNCTION()
	void AnimNotify_JumpStart();
	UFUNCTION()
	void AnimNotify_JumpEnd();
	UFUNCTION()
	void AnimNotify_FallStart();	// 점프 없이 그냥 떨어졌을 때

	UFUNCTION()
	void AnimNotify_Pause_JumpAtk();

	// 원거리공격 발사
	UFUNCTION()
	void AnimNotify_ShotProjectile();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _DT) override;

};
