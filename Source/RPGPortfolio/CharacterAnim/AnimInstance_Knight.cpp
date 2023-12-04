// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Knight.h"
#include "Animation/AnimNode_StateMachine.h"

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
		return;
	}

	// bIsMove = Idle->Move Trigger
	fMoveSpeed = m_Movement->Velocity.Size2D();
	if (0.f < fMoveSpeed && !m_Movement->GetCurrentAcceleration().IsZero())
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
	}

	vLocalVelocity = m_Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity);

	bIsInAir = m_Movement->IsFalling();

	if (bIsGuard && !bIsInAir)
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight + _DT * 9.f, 0.f, 1.f);
	}
	else
	{
		fGuardBlendWeight = FMath::Clamp(fGuardBlendWeight - _DT * 9.f, 0.f, 1.f);
	}

	if (bIsAttack)
	{
		fAttackBlendWeight = 1.f;
	}
	else
	{
		fAttackBlendWeight = 0.f;
	}

	const FAnimNode_StateMachine* state = GetStateMachineInstanceFromName(FName("Main"));
	if (FName("Idle/Move").IsEqual(state->GetCurrentStateName()))
	{
		m_Player->SetbEnableJump(false);
		m_Player->SetbEnableMove(true);
	}
	else
	{
		if (FName("Land").IsEqual(state->GetCurrentStateName()))
		{
			m_Player->SetbEnableMove(false);
		}
		m_Player->SetbEnableJump(true);
	}
}

void UAnimInstance_Knight::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_HitCheckStart()
{
	m_Player->SetbAtkTrace(true);
}

void UAnimInstance_Knight::AnimNotify_HitCheckEnd()
{
	m_Player->SetbAtkTrace(false);
}

void UAnimInstance_Knight::AnimNotify_DodgeEnd()
{
}
