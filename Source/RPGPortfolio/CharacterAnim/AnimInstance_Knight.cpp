// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Knight.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

void UAnimInstance_Knight::NativeInitializeAnimation()
{
}

void UAnimInstance_Knight::NativeBeginPlay()
{
	m_Player = Cast<APlayer_Base_Knight>(GetOwningActor());

	if (IsValid(m_Player))
	{
		m_Movement = m_Player->GetCharacterMovement();
	}
}

void UAnimInstance_Knight::NativeUpdateAnimation(float _DT)
{
	if (!IsValid(m_Movement) || !IsValid(m_Player))
	{
		m_Player = Cast<APlayer_Base_Knight>(GetOwningActor());

		if (IsValid(m_Player))
		{
			m_Movement = m_Player->GetCharacterMovement();
		}
		return;
	}
	vLocalVelocity.Z = m_Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity).Z;

	// bIsMove = Idle->Move Trigger
	fMoveSpeed = m_Movement->Velocity.Size2D();
	if (0.f < fMoveSpeed && !m_Movement->GetCurrentAcceleration().IsZero())
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
		vLocalVelocity.X = 0.f;
		vLocalVelocity.Y = 0.f;
	}

	// 가드 모션
	if (bIsGuard)
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight + _DT * 10.f, 0.f, 1.f);

		if ( fGuardBlendWeight >= 1.f )
		{
			m_Player->SetbToggleGuard(true);
		}
	}
	else
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight - _DT * 15.f, 0.f, 1.f);
	}
}

// 공격 몽타주 시작
//void UAnimInstance_Knight::AnimNotify_AtkSectionStart()
//{
//	m_Player->SetbNoInputInAtk(true);
//	m_Player->SetbAtkRotate(false);
//	OnAttackRotate.Broadcast();
//}

// 다음 공격 입력 시작
void UAnimInstance_Knight::AnimNotify_NextCheckStart()
{
	m_Player->SetbNextAtkCheck(true);
	m_Player->SetbAtkRotate(true);
}

// 다음 공격 입력 끝
void UAnimInstance_Knight::AnimNotify_NextCheckEnd()
{
	m_Player->SetbNextAtkCheck(false);
	m_Player->SetbInvalidInput(false);
	m_Player->SetbAtkRotate(false);
	m_Player->SetvAtkDirZero();
}

void UAnimInstance_Knight::AnimNotify_HitCheckStart()
{
	m_Player->SetbAtkTrace(true);
}

void UAnimInstance_Knight::AnimNotify_HitCheckEnd()
{
	m_Player->SetbAtkTrace(false);
	// 공격 대상 배열 초기화
	m_Player->EmptyHitActorArr();
	// 공격 판정 끝난 뒤에 공격 입력이 들어와야만 다음 콤보로 넘어가도록
	//m_Player->SetbAtkToggle(false);

}

void UAnimInstance_Knight::AnimNotify_MoveStart()
{
	OnAttackMove.Broadcast();
}

// 무적 프레임 시작
void UAnimInstance_Knight::AnimNotify_DodgeStart()
{
	OnDodgeTimeCheck.Broadcast(true);
}
// 무적 프레임 끝
void UAnimInstance_Knight::AnimNotify_DodgeEnd()
{
	OnDodgeTimeCheck.Broadcast(false);
}

void UAnimInstance_Knight::AnimNotify_DodgeAnimEnd()
{
	m_Player->SetbDodging(false);
	m_Player->SetbInvalidInput(false);
}

void UAnimInstance_Knight::AnimNotify_JumpStart()
{
	m_Player->SetbIsJumped(true);
}

void UAnimInstance_Knight::AnimNotify_JumpEnd()
{
	m_Player->SetbIsJumped(false);
}

void UAnimInstance_Knight::AnimNotify_FallStart()
{
	m_Player->SetbIsJumped(true);
}

void UAnimInstance_Knight::AnimNotify_Pause_JumpAtk()
{
	Montage_Pause();
	OnJumpAtk.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_ShotProjectile()
{
	m_Player->ShotProjectile();
}

void UAnimInstance_Knight::AnimNotify_InvalidInput()
{
	m_Player->SetbInvalidInput(true);
}

void UAnimInstance_Knight::AnimNotify_ValidInput()
{
	m_Player->SetbInvalidInput(false);
}
