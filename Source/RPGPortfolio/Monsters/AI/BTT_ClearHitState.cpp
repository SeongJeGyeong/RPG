// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ClearHitState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_ClearHitState::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	AAIController* pController = _OwnComp.GetAIOwner();

	pController->GetBlackboardComponent()->SetValueAsBool(TEXT("bHitted"), false);

	return EBTNodeResult::Succeeded;
}
