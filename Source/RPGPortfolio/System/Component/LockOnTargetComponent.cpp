// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnTargetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "../../Monsters/Monster_Base.h"
#include "../../Monsters/Boss_Base.h"

ULockOnTargetComponent::ULockOnTargetComponent()
{
	SetCollisionProfileName(FName("LOT"));

}

void ULockOnTargetComponent::SetLockOn(bool _LockOn)
{
	if (GetOwner()->IsA(AMonster_Base::StaticClass()))
	{
		AMonster_Base* pMonster = Cast<AMonster_Base>(GetOwner());
		pMonster->SetbLockedOn(_LockOn);
	}
	else if (GetOwner()->IsA(ABoss_Base::StaticClass()))
	{
		ABoss_Base* pBoss = Cast<ABoss_Base>(GetOwner());
		pBoss->SetbLockedOn(_LockOn);
	}
}

bool ULockOnTargetComponent::IsOwnerDead()
{
	if (GetOwner()->IsA(AMonster_Base::StaticClass()))
	{
		AMonster_Base* pMonster = Cast<AMonster_Base>(GetOwner());
		if (pMonster->GetbIsDead())
		{
			return true;
		}
	}
	else if (GetOwner()->IsA(ABoss_Base::StaticClass()))
	{
		ABoss_Base* pBoss = Cast<ABoss_Base>(GetOwner());
		if (pBoss->GetbIsDead())
		{
			return true;
		}
	}

	return false;
}
