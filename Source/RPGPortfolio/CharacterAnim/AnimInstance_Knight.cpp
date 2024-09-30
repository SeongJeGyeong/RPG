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
		vLocalVelocity.X = Player->GetfForwardSpeed();
		vLocalVelocity.Y = Player->GetfRightSpeed();
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

	/*if (!IsValid(m_Movement) || !IsValid(m_Player))
	{
		m_Player = Cast<APlayer_Base_Knight>(GetOwningActor());

		if (IsValid(m_Player))
		{
			m_Movement = m_Player->GetCharacterMovement();
		}
		return;
	}*/
	//vLocalVelocity.Z = m_Player->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity).Z;

	// bIsMove = Idle->Move Trigger
	//fMoveSpeed = m_Movement->Velocity.Size2D();
	//if (0.f < fMoveSpeed && !m_Movement->GetCurrentAcceleration().IsZero())
	//{
	//	bIsMove = true;
	//}
	//else
	//{
	//	bIsMove = false;
	//	//vLocalVelocity.X = 0.f;
	//	//vLocalVelocity.Y = 0.f;
	//}
}

void UAnimInstance_Knight::AnimNotify_NextAttackStart()
{
	m_Player->SetbNextAtkStart(true);
}

// 다음 공격 입력 끝
void UAnimInstance_Knight::AnimNotify_NextCheckEnd()
{
	m_Player->SetbNextAtkCheck(false);
	m_Player->SetState(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_HitCheckStart()
{
	//m_Player->HitBoxActivate(true);
	m_Player->SetbAtkTrace(true);
	m_Player->SetbNextAtkCheck(true);
}

void UAnimInstance_Knight::AnimNotify_HitCheckEnd()
{
	//m_Player->HitBoxActivate(false);
	m_Player->SetbAtkTrace(false);
	// 공격 대상 배열 초기화
	m_Player->EmptyHitActorArr();
	m_Player->ResetPrevTraceLoc();
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
	m_Player->SetState(EPlayerStateType::IDLE);
}

void UAnimInstance_Knight::AnimNotify_JumpStart()
{
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

void UAnimInstance_Knight::AnimNotify_InvalidInput()
{
	//m_Player->SetbInvalidInput(true);
}

void UAnimInstance_Knight::AnimNotify_ValidInput()
{
	//m_Player->SetbInvalidInput(false);
	m_Player->SetState(EPlayerStateType::IDLE);
}