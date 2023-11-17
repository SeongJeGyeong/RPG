// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnLorR.h"
#include "../AIC_Monster_Base.h"
#include "../../../Monsters/Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../MonsterAnim/AnimInstance_Boss_Base.h"

UBTT_TurnLorR::UBTT_TurnLorR()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_TurnLorR::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	AAIController* pController = _OwnComp.GetAIOwner();
	if (!IsValid(pController))
	{
		return EBTNodeResult::Failed;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if (nullptr == pMonster)
	{
		return EBTNodeResult::Failed;
	}

	m_AnimInst = Cast<UAnimInstance_Boss_Base>(pMonster->GetMesh()->GetAnimInstance());
	if (!IsValid(m_AnimInst))
	{
		return EBTNodeResult::Failed;
	}

	int32 iDir = pController->GetBlackboardComponent()->GetValueAsInt(TEXT("TargetDirection"));
	
	if (iDir == 0)
	{
		return EBTNodeResult::Failed;
	}

	m_AnimInst->PlayTurnMontage(iDir);

	return EBTNodeResult::InProgress;
}

void UBTT_TurnLorR::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (!m_AnimInst->bIsTurn)
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}
}
