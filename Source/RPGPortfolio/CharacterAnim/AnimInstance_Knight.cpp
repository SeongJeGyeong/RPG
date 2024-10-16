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
		FVector Vec = Player->GetActorRotation().UnrotateVector(Player->GetLastMovementInputVector());

		fGuardBlendWeight = Player->GetfGuardWeight();
		vLocalVelocity.X = Vec.GetSafeNormal().X;
		vLocalVelocity.Y = Vec.GetSafeNormal().Y;
		if ( Player->GetCharacterMovement()->MaxWalkSpeed >= 600.f )
		{
			vLocalVelocity.X *= 2;
			vLocalVelocity.Y *= 2;
		}
		bIsLockOn = Player->GetbIsLockOn();
		vLocalVelocity.Z = Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(Movement->Velocity).Z;
		fMoveSpeed = Movement->Velocity.Size2D();
		vCurAcceleration = Movement->GetCurrentAcceleration();
	}
}

void FAnimInstanceProxy_Knight::Update(float _DeltaSeconds)
{
	Super::Update(_DeltaSeconds);

	if ( Player )
	{
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

void FAnimInstanceProxy_Knight::PostUpdate(UAnimInstance* _InAnimInstance) const
{
	Super::PostUpdate(_InAnimInstance);
}

void UAnimInstance_Knight::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimInstance_Knight::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Player = Cast<APlayer_Base_Knight>(GetOwningActor());

	if (IsValid(m_Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("애님 인스턴스 : 플레이어 캐릭터 참조 성공"));
	}
}

void UAnimInstance_Knight::NativeUpdateAnimation(float _DT)
{
	Super::NativeUpdateAnimation(_DT);
}

void UAnimInstance_Knight::AnimNotify_NextAttackStart()
{
	m_Player->SetState(EPlayerStateType::ATTACK_WAIT);
}

// 다음 공격 입력 끝
void UAnimInstance_Knight::AnimNotify_NextCheckEnd()
{
	m_Player->SetbEnableAtkInput(false);
	m_Player->SetState(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_HitCheckStart()
{
	m_Player->SetbAtkTrace(true);
	m_Player->SetbEnableAtkInput(true);
}

void UAnimInstance_Knight::AnimNotify_HitCheckEnd()
{
	m_Player->SetbAtkTrace(false);
	// 공격 대상 배열 초기화
	m_Player->EmptyHitActorArr();
	m_Player->ResetPrevTraceLoc();
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
	m_Player->SetState(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_JumpEnd()
{
	m_Player->SetState(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_FallStart()
{
	m_Player->SetState(EPlayerStateType::JUMP);
}

void UAnimInstance_Knight::AnimNotify_Pause_JumpAtk()
{
	Montage_Pause();
	OnJumpAtk.Broadcast();
}

void UAnimInstance_Knight::AnimNotify_JumpAtkEnd()
{
	m_Player->SetState(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_ShotProjectile()
{
	m_Player->ShotProjectile();
}

void UAnimInstance_Knight::AnimNotify_ValidInput()
{
	m_Player->SetState(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_Dead()
{
	OnDead.Broadcast();
}
