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

void UAnim_GreaterSpider::AnimNotify_RushStart()
{
	OnRushAttack.Broadcast(true);
}

void UAnim_GreaterSpider::AnimNotify_RushEnd()
{
	OnRushAttack.Broadcast(false);
}

void UAnim_GreaterSpider::AnimNotify_AttackStart()
{
	ABoss_GreaterSpider* pSpider = Cast<ABoss_GreaterSpider>(TryGetPawnOwner());
	pSpider->SetbAtkTrace(true);
}

void UAnim_GreaterSpider::AnimNotify_AttackEnd()
{
	ABoss_GreaterSpider* pSpider = Cast<ABoss_GreaterSpider>(TryGetPawnOwner());
	pSpider->SetbAtkTrace(false);
	pSpider->EmptyHitArr();
}

void UAnim_GreaterSpider::AnimNotify_ShotProj()
{
	UE_LOG(LogTemp, Warning, TEXT("ShotProj"));
	ABoss_GreaterSpider* pSpider = Cast<ABoss_GreaterSpider>(TryGetPawnOwner());
	pSpider->RangedAttack();
}
