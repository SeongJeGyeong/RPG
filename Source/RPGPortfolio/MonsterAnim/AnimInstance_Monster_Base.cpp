// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Monster_Base.h"
#include "../Monsters/Monster_Base.h"

void UAnimInstance_Monster_Base::NativeInitializeAnimation()
{
	m_Monster = Cast<AMonster_Base>(TryGetPawnOwner());
}

void UAnimInstance_Monster_Base::NativeBeginPlay()
{
}

void UAnimInstance_Monster_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	if (!IsValid(m_Monster))
	{
		return;
	}

	m_State = m_Monster->GetState();
	
}
