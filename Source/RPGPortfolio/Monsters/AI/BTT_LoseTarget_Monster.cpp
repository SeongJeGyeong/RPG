// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LoseTarget_Monster.h"
#include "../Monster_Base.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTT_LoseTarget_Monster::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());

	if (IsValid(pMonster))
	{
		pMonster->ChangeState(EMONSTER_STATE::LOOKAROUND);
		//_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("RecentPosSet"), false);
	}

	return Super::ExecuteTask(_OwnComp, _NodeMemory);
}
