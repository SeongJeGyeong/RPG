// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Knight.h"

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
		GuardBlendWeight = FMath::Clamp(GuardBlendWeight + _DT * 9.f, 0.f, 1.f);
		UE_LOG(LogTemp, Warning, TEXT("BlendWeight : %f"), GuardBlendWeight);
	}
	else
	{
		GuardBlendWeight = FMath::Clamp(GuardBlendWeight - _DT * 8.f, 0.f, 1.f);
	}

	bIsTargeting = m_Player->bLockOn;

	//FVector vCameraLocation = m_Player->GetCamera()->GetComponentLocation();
	//FVector vCameraLookLocation = m_Player->GetCamera()->GetForwardVector() * 1000;

	//vCameraLookAt = vCameraLocation + vCameraLookLocation;

}
