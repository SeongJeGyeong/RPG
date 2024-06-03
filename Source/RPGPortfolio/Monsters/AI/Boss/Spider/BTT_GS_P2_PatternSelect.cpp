// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GS_P2_PatternSelect.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_GS_P2_PatternSelect::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	int iNum = FMath::RandRange(1, 10);
	if ( iNum > 7 )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bAtkRangeType"), true);
		UE_LOG(LogTemp, Warning, TEXT("RangeType"));
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bAtkRangeType"), false);
		UE_LOG(LogTemp, Warning, TEXT("MeleeType"));
	}

	return EBTNodeResult::Succeeded;
}
