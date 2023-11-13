// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RandomPattern.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_RandomPattern::UBTS_RandomPattern()
{
	NodeName = TEXT("SelectRandomPattern");
	Interval = 1.f;

	bNotifyBecomeRelevant = true;
}

void UBTS_RandomPattern::OnBecomeRelevant(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::OnBecomeRelevant(_OwnComp, _NodeMemory);

	int iNum = FMath::RandRange(1, 10);

	if (iNum <= 3)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("PatternKey"), 1);
	}
	else if (iNum <= 8)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("PatternKey"), 2);
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("PatternKey"), 3);
	}
}
