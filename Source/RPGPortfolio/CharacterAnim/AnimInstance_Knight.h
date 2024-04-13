// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Characters/Player_Base_Knight.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Knight.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate); // 다음콤보 체크 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnInvincibleTimeCheckDelegate); // 무적시간 체크 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnAttackMoveDelegate);	// 공격 중 이동 델리게이트

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FVector	vCameraLookAt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float fGuardBlendWeight;

	// IK용 변수
	UPROPERTY()
	TArray<AActor*> IgnoreActorArr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fRootDisplacement;		// 루트 본의 높이 위치

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fLineOutCapsule = 50.f;	// 캡슐 밖으로 뻗어나온 라인트레이스 길이

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fIKInterpSpeed = 10.f;	// IK가 적용되는 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	FRotator rRRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	FRotator rLRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fRIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fLIK;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsGuard;

	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnInvincibleTimeCheckDelegate OnInvincibleTimeCheck;
	FOnAttackMoveDelegate	OnAttackMove;

	// 다음콤보 체크
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_HitCheckStart();

	UFUNCTION()
	void AnimNotify_HitCheckEnd();

	UFUNCTION()
	void AnimNotify_MoveStart();

	UFUNCTION()
	void AnimNotify_MoveEnd();

	UFUNCTION()
	void AnimNotify_DodgeStart();

	UFUNCTION()
	void AnimNotify_DodgeEnd();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _DT) override;
	
	// Foot IK용 함수들
	void FootIK(float _DeltaTime);
	TTuple<bool, float> CapsuleDistance(FName _SocketName, ACharacter* _Char);
	TTuple<bool, float, FVector> FootLineTrace(FName _SocketName, ACharacter* _Char);

};
