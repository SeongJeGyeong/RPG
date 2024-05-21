// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Monster_Hitted.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Monster_Hitted::UBTT_Monster_Hitted()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Monster_Hitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* pController = Cast<AAIController>(OwnerComp.GetOwner());

	pController->StopMovement();

	return EBTNodeResult::InProgress;
}

void UBTT_Monster_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("bHitted")))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
