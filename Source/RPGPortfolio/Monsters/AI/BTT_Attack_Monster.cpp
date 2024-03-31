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

	pMonster->MonsterAttackNormal();

	return EBTNodeResult::Succeeded;
}

void UBTT_Attack_Monster::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//// 타겟(플레이어) 가져옴
	//ACharacter* pTarget = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	//if (!IsValid(pTarget))
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}

	//float fAtkRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName("AtkRange"));

	//AAIController* pController = OwnerComp.GetAIOwner();
	//AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	//if (!IsValid(pMonster))
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}

	//float Distance = FVector::Distance(pTarget->GetActorLocation(), pMonster->GetActorLocation());

	//// 몬스터와 타겟 사이의 거리가 공격범위 밖이면
	//if (Distance > fAtkRange)
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	return;
	//}
}
