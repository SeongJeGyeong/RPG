// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GS_Attack.h"
#include "../../../Boss_GreaterSpider.h"
#include "../../../../MonsterAnim/Anim_GreaterSpider.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_GS_Attack::UBTT_GS_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_GS_Attack::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	AAIController* pController = _OwnComp.GetAIOwner();
	if (!IsValid(pController))
	{
		return EBTNodeResult::Succeeded;
	}

	ABoss_GreaterSpider* pBoss = Cast<ABoss_GreaterSpider>(pController->GetPawn());
	if (!IsValid(pBoss))
	{
		return EBTNodeResult::Succeeded;
	}

	int32 fAttackAngle = pController->GetBlackboardComponent()->GetValueAsInt(TEXT("AttackDirection"));

	// 왼쪽 공격
	if ( fAttackAngle == 1 )
	{
		pBoss->PlayAttackMontage(EGreaterSpider_STATE::LEFTATTACK);
		UE_LOG(LogTemp, Warning, TEXT("LeftAttack"));
	}
	// 오른쪽 공격
	else if( fAttackAngle == 2 )
	{
		pBoss->PlayAttackMontage(EGreaterSpider_STATE::RIGHTATTACK);
		UE_LOG(LogTemp, Warning, TEXT("RightAttack"));
	}
	// 정면 공격
	else
	{
		pBoss->PlayAttackMontage(EGreaterSpider_STATE::CENTERATTACK);
		UE_LOG(LogTemp, Warning, TEXT("CenterAttack"));
	}


	return EBTNodeResult::InProgress;
}

void UBTT_GS_Attack::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	ABoss_GreaterSpider* pBoss = Cast<ABoss_GreaterSpider>(_OwnComp.GetAIOwner()->GetPawn());
	if (!IsValid(pBoss))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	UAnim_GreaterSpider* AnimInst = Cast<UAnim_GreaterSpider>(pBoss->GetMesh()->GetAnimInstance());
	if (!IsValid(AnimInst))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	if ( !AnimInst->IsAnyMontagePlaying() )
	{
		// idle 상태로
		pBoss->SetiTurnDir(0);
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
	
}
