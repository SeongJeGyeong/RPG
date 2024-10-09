// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GS_P2_PatternSelect.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_GS_P2_PatternSelect::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);


	int iNum = FMath::RandRange(1, 10);
	if ( iNum > 7 )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("Phase2Pattern"), 3);
	}
	else if ( iNum > 4 && !_OwnComp.GetBlackboardComponent()->GetValueAsBool(FName("InAtkRange")))
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("Phase2Pattern"), 2);
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("Phase2Pattern"), 1);
	}

	return EBTNodeResult::Succeeded;
}
