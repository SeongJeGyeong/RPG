// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UA_Atk_Melee.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Monster_UndeadAssassin.h"

UBTT_UA_Atk_Melee::UBTT_UA_Atk_Melee()
{
}

EBTNodeResult::Type UBTT_UA_Atk_Melee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* pController = Cast<AAIController>(OwnerComp.GetAIOwner());
	AMonster_UndeadAssassin* pMonster = Cast<AMonster_UndeadAssassin>(pController->GetPawn());
	if ( pMonster != nullptr )
	{
		pMonster->MonsterAttackMelee();
	}

	return EBTNodeResult::Succeeded;
}