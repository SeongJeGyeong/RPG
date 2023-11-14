// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Attack.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Griffon_Attack::UBTT_Griffon_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Griffon_Attack::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	int32 PatternNum = _OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("PatternKey"));

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if (nullptr == pMonster)
	{
		return EBTNodeResult::Succeeded;
	}

	switch (PatternNum)
	{
	case 1:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO1);
		break;
	case 2:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO2);
		break;
	case 3:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO3);
		break;
	default:
		break;
	}

	return EBTNodeResult::Succeeded;
}

void UBTT_Griffon_Attack::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);
}
