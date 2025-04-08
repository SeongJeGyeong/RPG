// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "Animation/AnimInstanceProxy.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Knight.generated.h"

class APlayer_Base_Knight;
class UCharacterMovementComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInvincibleDelegate, bool);				// 무적시간 체크 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnableComboInputDelegate, const bool&);	// 콤보공격 입력 유효성 체크 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetAtkTraceDelegate, const bool&);		// 공격 트레이스 활성화 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnJumpAtkDelegate);									// 점프공격 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnDeadDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetStateDelegate, EPlayerStateType);		// 플레이어 스테이트 변경 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnShotProjDelegate);								// 투사체 발사 델리게이트

USTRUCT(BlueprintType)
struct RPGPORTFOLIO_API FAnimInstanceProxy_Knight : public FAnimInstanceProxy
{
	GENERATED_USTRUCT_BODY()

protected:
	virtual void InitializeObjects(UAnimInstance* _InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* _InAnimInstance, float _DeltaSeconds) override;	// 게임 스레드
	virtual void Update(float _DeltaSeconds) override;										// 워커 스레드

	UPROPERTY()
	TObjectPtr<UAnimInstance_Knight> AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ref")
	TObjectPtr<APlayer_Base_Knight> Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ref")
	TObjectPtr<UCharacterMovementComponent> Movement;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Data")
	float fMaxSpeed;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Data")
	float fMoveSpeed;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Data")
	FVector	vLocalVelocity;	// 이동 블렌드 스페이스용 벡터

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Data")
	FVector	vCurAcceleration;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Data")
	float fGuardBlendWeight; // 가드 애니메이션 블렌드용 수치

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Data")
	bool bIsLockOn;
};

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstance_Knight : public UAnimInstance
{
	GENERATED_BODY()

public:
	friend struct FAnimInstanceProxy_Knight;

protected:
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return &m_AnimInst_Proxy; }
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* _InProxy) override {}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Proxy")
	FAnimInstanceProxy_Knight m_AnimInst_Proxy;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Data")
	float m_fMoveSpeed;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Data")
	bool m_bIsMove;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Data")
	bool m_bIsLockOn;

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Data")
	FVector	m_vLocalVelocity;	// 이동 블렌드 스페이스용 벡터

	UPROPERTY(Transient, BlueprintReadWrite, Category = "Data")
	float m_fGuardBlendWeight;// 가드 애니메이션 블렌드용 수치

public:
	FOnInvincibleDelegate	OnInvincibleState;
	FOnEnableComboInputDelegate OnEnableComboInput;
	FOnSetAtkTraceDelegate	OnSetAtkTrace;
	FOnJumpAtkDelegate		OnJumpAtk;
	FOnDeadDelegate			OnDead;
	FOnSetStateDelegate		OnSetState;
	FOnShotProjDelegate		OnShotProj;

	UFUNCTION()
	void AnimNotify_HitCheckStart();
	UFUNCTION()
	void AnimNotify_HitCheckEnd();

	// 다음콤보 체크
	UFUNCTION()
	void AnimNotify_NextAttackStart();
	UFUNCTION()
	void AnimNotify_NextCheckEnd();

	UFUNCTION()
	void AnimNotify_InvincibleOn();
	UFUNCTION()
	void AnimNotify_InvincibleOff();

	UFUNCTION()
	void AnimNotify_DodgeAnimEnd();

	UFUNCTION()
	void AnimNotify_JumpEnd();
	UFUNCTION()
	void AnimNotify_FallStart();	// 점프 없이 그냥 떨어졌을 때

	UFUNCTION()
	void AnimNotify_Pause_JumpAtk();
	UFUNCTION()
	void AnimNotify_JumpAtkEnd();

	// 원거리공격 발사
	UFUNCTION()
	void AnimNotify_ShotProjectile();

	UFUNCTION()
	void AnimNotify_ValidInput();

	UFUNCTION()
	void AnimNotify_Dead();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _DT) override;

};
