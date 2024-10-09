// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RandomPattern.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../AIC_Monster_Base.h"

UBTT_RandomPattern::UBTT_RandomPattern()
{
}

EBTNodeResult::Type UBTT_RandomPattern::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	int iNum = FMath::RandRange(1, 10);

	if (iNum <= 3)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(AAIC_Monster_Base::PatternNumber, 1);
	}
	else if (iNum <= 8)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(AAIC_Monster_Base::PatternNumber, 2);
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(AAIC_Monster_Base::PatternNumber, 3);
	}

	return EBTNodeResult::Succeeded;
}
