// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_GreaterSpider.h"
#include "../Monsters/Boss_GreaterSpider.h"

void UAnim_GreaterSpider::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnim_GreaterSpider::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Boss = Cast<ABoss_GreaterSpider>(TryGetPawnOwner());
	if (!IsValid(m_Boss))
	{
		UE_LOG(LogTemp, Error, TEXT("Anim_GreaterSpider : 애님클래스 오너 찾지 못함"));
	}
	else
	{
		m_Movement = m_Boss->GetCharacterMovement();
	}
}

void UAnim_GreaterSpider::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);
}
