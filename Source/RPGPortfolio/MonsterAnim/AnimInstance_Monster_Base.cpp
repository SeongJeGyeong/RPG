// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Monster_Base.h"
#include "../Monsters/Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UAnimInstance_Monster_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	m_Monster = Cast<AMonster_Base>(TryGetPawnOwner());
}

void UAnimInstance_Monster_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UAnimInstance_Monster_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);

	if (!IsValid(m_Monster))
	{
		return;
	}

	m_State = m_Monster->GetState();
}

void UAnimInstance_Monster_Base::AnimNotify_HitCheckStart()
{
	m_Monster->SetbAtkTrace(true);
}

void UAnimInstance_Monster_Base::AnimNotify_HitCheckEnd()
{
	m_Monster->SetbAtkTrace(false);
}

void UAnimInstance_Monster_Base::AnimNotify_Hit_Start()
{
	/*AAIController* pAIController = Cast<AAIController>(m_Monster->GetController());

	if (IsValid(pAIController))
	{
		if (pAIController->GetBlackboardComponent())
		{
			pAIController->GetBlackboardComponent()->SetValueAsBool(FName("WasHit"), true);
		}
	}*/
}

void UAnimInstance_Monster_Base::AnimNotify_Hit_End()
{
	/*AAIController* pAIController = Cast<AAIController>(m_Monster->GetController());

	if ( IsValid(pAIController) )
	{
		if ( pAIController->GetBlackboardComponent() )
		{
			pAIController->GetBlackboardComponent()->SetValueAsBool(FName("WasHit"), false);
		}
	}*/
}
