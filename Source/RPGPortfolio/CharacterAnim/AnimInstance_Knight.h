// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Characters/Player_Base_Knight.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Knight.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate); // 다음콤보 체크 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam (FOnDodgeTimeCheckDelegate, bool); // 무적시간 체크 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam (FOnAttackMoveDelegate, bool);	// 공격 중 이동 델리게이트

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	bool bIsGuard;			// 가드액션 여부

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float fGuardBlendWeight;// 가드 애니메이션 블렌드용 수치

	// IK용 변수
	UPROPERTY()
	TArray<AActor*> IgnoreActorArr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fRootDisplacement;		// 루트 본의 높이 위치

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fLineOutCapsule = 50.f;	// 캡슐 밖으로 뻗어나온 라인트레이스 길이

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fIKInterpSpeed = 5.f;	// IK가 적용되는 속도

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	FRotator rRRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	FRotator rLRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fRIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = ( AllowPrivateAccess = "true" ))
	float fLIK;

public:
	bool GetbIsGuard() { return bIsGuard; }
	void SetbIsGaurd(bool _IsGuard) { bIsGuard = _IsGuard; }

	void SetLocalVelocityXY(FVector2D _Velocity) { vLocalVelocity.X = _Velocity.X; vLocalVelocity.Y = _Velocity.Y; }

	//void TurnOnItemUseBlend(float _BlentWeight) { fItemBlendWeight = _BlentWeight; }

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnDodgeTimeCheckDelegate OnDodgeTimeCheck;
	FOnAttackMoveDelegate	OnAttackMove;

	UFUNCTION()
	void AnimNotify_AtkSectionStart();

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
	void AnimNotify_MoveEnd();

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

	// 원거리공격 발사
	UFUNCTION()
	void AnimNotify_ShotProjectile();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _DT) override;
	
	// Foot IK용 함수들
	void FootIK(float _DeltaTime);
	TTuple<bool, float> CapsuleDistance(FName _SocketName, ACharacter* _Char);
	TTuple<bool, float, FVector> FootLineTrace(FName _SocketName, ACharacter* _Char);

};
