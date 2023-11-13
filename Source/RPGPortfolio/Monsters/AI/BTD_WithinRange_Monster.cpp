// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_WithinRange_Monster.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../Monster_Base.h"

UBTD_WithinRange_Monster::UBTD_WithinRange_Monster()
{
	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_WithinRange_Monster, m_TargetKey), AActor::StaticClass());
	m_RangeKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_WithinRange_Monster, m_RangeKey));
}

bool UBTD_WithinRange_Monster::CalculateRawConditionValue(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) const
{
	if (m_TargetKey.IsNone())
	{
		return false;
	}

	UObject* pObject = _OwnComp.GetBlackboardComponent()->GetValueAsObject(m_TargetKey.SelectedKeyName);
	if (!IsValid(pObject))
	{
		return false;
	}

	AActor* pTarget = Cast<AActor>(pObject);

	float fRange = m_Range;
	if (!m_RangeKey.IsNone())
	{
		fRange = _OwnComp.GetBlackboardComponent()->GetValueAsFloat(m_RangeKey.SelectedKeyName);
	}

	AAIController* pController = _OwnComp.GetAIOwner();

	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());

	if (IsValid(pMonster))
	{
		float Distance = FVector::Distance(pTarget->GetActorLocation(), pMonster->GetActorLocation());
		if (Distance < fRange)
		{
			return true;
		}
	}

	return false;
}
