// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Boss_Base.h"
#include "../Characters/Player_Base_Knight.h"
#include "Kismet/GameplayStatics.h"

void UAnimInstance_Boss_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimInstance_Boss_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Boss = Cast<ABoss_Base>(TryGetPawnOwner());
	if (IsValid(m_Boss))
	{
		m_Movement = m_Boss->GetCharacterMovement();
	}
}

void UAnimInstance_Boss_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);

	// 애니메이션 블루프린트에서도 함수가 호출되는것으로 보임
	if (!IsValid(m_Boss) || !IsValid(m_Movement))
	{
		m_Boss = Cast<ABoss_Base>(TryGetPawnOwner());
		if (IsValid(m_Boss))
		{
			m_Movement = m_Boss->GetCharacterMovement();
		}
		return;
	}

	// 인티저 포즈 블렌딩용
	iRotateDir = m_Boss->GetiTurnDir();

	FQuat Orientation = m_Movement->UpdatedComponent->GetComponentQuat();
	fMoveSpeed = m_Movement->Velocity.Size2D();
	if (0.f < fMoveSpeed)
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
	}
	qOldOrientation = Orientation;

	vLocalVelocity = m_Boss->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity);
}