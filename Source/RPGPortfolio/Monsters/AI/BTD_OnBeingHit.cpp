// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_OnBeingHit.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_OnBeingHit::UBTD_OnBeingHit()
{
	m_WasHit.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTD_OnBeingHit, m_WasHit));
}

bool UBTD_OnBeingHit::CalculateRawConditionValue(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) const
{
	Super::CalculateRawConditionValue(_OwnComp, _NodeMemory);

	if (m_WasHit.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("WasHit 세팅안됨"));
		return true;
	}

	bool washit = _OwnComp.GetBlackboardComponent()->GetValueAsBool(m_WasHit.SelectedKeyName);
	if (washit)
	{
		UE_LOG(LogTemp, Warning, TEXT("WasHit true"));
		return false;
	}

	return true;
}
