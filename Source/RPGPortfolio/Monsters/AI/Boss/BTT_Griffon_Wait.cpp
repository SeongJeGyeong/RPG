// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Wait.h"
#include "../../Monster_Griffon.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_Griffon_Wait::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	AMonster_Griffon* pMonster = Cast<AMonster_Griffon>(_OwnComp.GetAIOwner()->GetPawn());

	if (IsValid(pMonster))
	{
		pMonster->ChangeBossState(EBOSS_STATE::DEFAULT);
	}

	return Super::ExecuteTask(_OwnComp, _NodeMemory);
}
