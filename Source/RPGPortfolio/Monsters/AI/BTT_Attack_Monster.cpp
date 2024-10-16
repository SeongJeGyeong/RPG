// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack_Monster.h"
#include "../Monster_Base.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Attack_Monster::UBTT_Attack_Monster()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Attack_Monster::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* pController = Cast<AAIController>(OwnerComp.GetAIOwner());
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if ( pMonster != nullptr )
	{
		pMonster->MonsterAttackNormal();
	}

	return EBTNodeResult::Succeeded;
}

void UBTT_Attack_Monster::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);	
}
