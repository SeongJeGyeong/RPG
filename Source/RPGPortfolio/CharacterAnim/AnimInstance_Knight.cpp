// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Knight.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "../Characters/Player_Base_Knight.h"

void FAnimInstanceProxy_Knight::InitializeObjects(UAnimInstance* _InAnimInstance)
{
	Super::InitializeObjects(_InAnimInstance);

	if ( !_InAnimInstance )
	{
		return;
	}

	AnimInstance = Cast<UAnimInstance_Knight>(_InAnimInstance);
	Player = Cast<APlayer_Base_Knight>(_InAnimInstance->GetOwningActor());

	if (Player)
	{
		Movement = Player->GetCharacterMovement();
	}

}

void FAnimInstanceProxy_Knight::PreUpdate(UAnimInstance* _InAnimInstance, float _DeltaSeconds)
{
	Super::PreUpdate(_InAnimInstance, _DeltaSeconds);

	if (Player)
	{
		fGuardBlendWeight = Player->GetfGuardWeight();
		bIsLockOn = Player->GetbIsLockOn();
		vLocalVelocity = Player->GetActorRotation().UnrotateVector(Player->GetLastMovementInputVector());
		vLocalVelocity.Z = Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(Movement->Velocity).Z;
		fMaxSpeed = Movement->MaxWalkSpeed;
		fMoveSpeed = Movement->Velocity.Size2D();
		vCurAcceleration = Movement->GetCurrentAcceleration();
	}
}

void FAnimInstanceProxy_Knight::Update(float _DeltaSeconds)
{
	Super::Update(_DeltaSeconds);

	if ( Player )
	{
		vLocalVelocity.X = vLocalVelocity.GetSafeNormal().X;
		vLocalVelocity.Y = vLocalVelocity.GetSafeNormal().Y;
		if ( fMaxSpeed >= 600.f )
		{
			vLocalVelocity.X *= 2;
			vLocalVelocity.Y *= 2;
		}

		if ( 0.f < fMoveSpeed && !vCurAcceleration.IsZero() )
		{
			AnimInstance->m_bIsMove = true;
		}
		else
		{
			AnimInstance->m_bIsMove = false;
		}

		AnimInstance->m_fMoveSpeed = fMoveSpeed;
		AnimInstance->m_vLocalVelocity = vLocalVelocity;
		AnimInstance->m_fGuardBlendWeight = fGuardBlendWeight;
		AnimInstance->m_bIsLockOn = bIsLockOn;
	}
}

void UAnimInstance_Knight::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimInstance_Knight::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UAnimInstance_Knight::NativeUpdateAnimation(float _DT)
{
	Super::NativeUpdateAnimation(_DT);
}

void UAnimInstance_Knight::AnimNotify_HitCheckStart()
{
	OnSetAtkTrace.Broadcast(true);
	OnEnableComboInput.Broadcast(true);
}

void UAnimInstance_Knight::AnimNotify_HitCheckEnd()
{
	OnSetAtkTrace.Broadcast(false);
}

// 다음 공격 시작
void UAnimInstance_Knight::AnimNotify_NextAttackStart()
{
	OnSetState.Broadcast(EPlayerStateType::ATTACK_WAIT);
}

// 다음 공격 입력 끝
void UAnimInstance_Knight::AnimNotify_NextCheckEnd()
{
	OnEnableComboInput.Broadcast(false);
	OnSetState.Broadcast(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_InvincibleOn()
{
	OnInvincibleState.Broadcast(true);
}

void UAnimInstance_Knight::AnimNotify_InvincibleOff()
{
	OnInvincibleState.Broadcast(false);
}

void UAnimInstance_Knight::AnimNotify_DodgeAnimEnd()
{
	OnSetState.Broadcast(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_JumpEnd()
{
	OnSetState.Broadcast(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_FallStart()
{
	OnSetState.Broadcast(EPlayerStateType::JUMP);
}

void UAnimInstance_Knight::AnimNotify_Pause_JumpAtk()
{
	Montage_Pause();
	OnJumpAtk.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_JumpAtkEnd()
{
	OnSetState.Broadcast(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_ShotProjectile()
{
	OnShotProj.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_ValidInput()
{
	OnSetState.Broadcast(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_Dead()
{
	OnDead.Broadcast();
}
