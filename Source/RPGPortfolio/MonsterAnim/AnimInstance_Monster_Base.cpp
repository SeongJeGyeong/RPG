// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Monster_Base.h"
#include "../Monsters/Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimInstance_Monster_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimInstance_Monster_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Monster = Cast<AMonster_Base>(TryGetPawnOwner());

	if ( !IsValid(m_Monster) )
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터애님에서 몬스터폰 캐스팅 실패"));
	}
	else
	{
		m_Movement = m_Monster->GetCharacterMovement();
	}
}

void UAnimInstance_Monster_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);

	if (!IsValid(m_Monster))
	{
		return;
	}

	vLocalVelocity = m_Monster->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity);

	if ( 0.f < m_Movement->Velocity.Size2D() )
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
	}
}

void UAnimInstance_Monster_Base::AnimNotify_HitCheckStart()
{
	m_Monster->SetActorTickEnabled(true);
	m_Monster->SetbAtkTrace(true);
}

void UAnimInstance_Monster_Base::AnimNotify_HitCheckEnd()
{
	m_Monster->SetbAtkTrace(false);
	m_Monster->SetActorTickEnabled(false);
}

void UAnimInstance_Monster_Base::AnimNotify_NextAtkCheck()
{
	Montage_JumpToSection(FName(TEXT("Combo2")));
}