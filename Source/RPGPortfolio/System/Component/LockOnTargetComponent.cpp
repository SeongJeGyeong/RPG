// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnTargetComponent.h"
#include "../../Monsters/Monster_Base.h"
#include "../../Monsters/Boss_Base.h"
#include "Components/WidgetComponent.h"

ULockOnTargetComponent::ULockOnTargetComponent()
{
	SetCollisionProfileName(FName("LOT"));

	m_LockOnMark = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));

	static ConstructorHelpers::FClassFinder<UUserWidget> LockOnUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Monster/BPC_UI_LockOnMarker.BPC_UI_LockOnMarker_C'"));
	if ( LockOnUI.Succeeded() )
	{
		m_LockOnMark->SetWidgetClass(LockOnUI.Class);
	}

	m_LockOnMark->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	m_LockOnMark->SetDrawSize(FVector2D(25.f, 25.f));
	m_LockOnMark->SetWidgetSpace(EWidgetSpace::Screen);
	m_LockOnMark->SetVisibility(false);

	SphereRadius = 5.f;
}

void ULockOnTargetComponent::SetLockOnMarkVisibility(bool _Visible)
{
	m_LockOnMark->SetVisibility(_Visible);
}

void ULockOnTargetComponent::SetLockOn(bool _LockOn)
{
	if (GetOwner()->IsA(AMonster_Base::StaticClass()))
	{
		AMonster_Base* pMonster = Cast<AMonster_Base>(GetOwner());
		// 몬스터 액터에 록온 효과를 활성화
		pMonster->SetMonLockedOn(_LockOn);
	}
	else if (GetOwner()->IsA(ABoss_Base::StaticClass()))
	{
		ABoss_Base* pBoss = Cast<ABoss_Base>(GetOwner());
		pBoss->SetBossLockedOn(_LockOn);
	}
	m_LockOnMark->SetVisibility(_LockOn);
}

bool ULockOnTargetComponent::IsOwnerDead()
{
	if (GetOwner()->IsA(AMonster_Base::StaticClass()))
	{
		AMonster_Base* pMonster = Cast<AMonster_Base>(GetOwner());
		if (pMonster->GetbMonDead())
		{
			return true;
		}
	}
	else if (GetOwner()->IsA(ABoss_Base::StaticClass()))
	{
		ABoss_Base* pBoss = Cast<ABoss_Base>(GetOwner());
		if (pBoss->GetbBossDead())
		{
			return true;
		}
	}

	return false;
}
