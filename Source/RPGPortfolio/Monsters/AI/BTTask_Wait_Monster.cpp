// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Wait_Monster.h"
#include "../Monsters_Base.h"
#include "../Monster_Base.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_Wait_Monster::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());

	if (IsValid(pMonster))
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("RecentPosSet"), false);
	}

	return Super::ExecuteTask(_OwnComp, _NodeMemory);
}
