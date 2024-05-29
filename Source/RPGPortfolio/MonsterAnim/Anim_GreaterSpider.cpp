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
}

void UAnim_GreaterSpider::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);
}

void UAnim_GreaterSpider::AnimNotify_DeadAnimEnd()
{
	ABoss_GreaterSpider* pSpider = Cast<ABoss_GreaterSpider>(TryGetPawnOwner());
	pSpider->DeadCollisionSetting();
}
