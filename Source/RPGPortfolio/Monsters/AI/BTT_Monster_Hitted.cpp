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
	UE_LOG(LogTemp, Warning, TEXT("Hit State Start"));
	return EBTNodeResult::InProgress;
}

void UBTT_Monster_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// bHitted가 false일 경우 태스크가 종료되므로 애초에 조건문에 들어갈 일은 없음
	// 만약을 대비해 넣어놓은 것
	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("bHitted")))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
