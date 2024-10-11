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
	if ( !IsValid(pController) )
	{
		return EBTNodeResult::Succeeded;
	}

	ABoss_GreaterSpider* pBoss = Cast<ABoss_GreaterSpider>(pController->GetPawn());
	if ( !IsValid(pBoss) )
	{
		return EBTNodeResult::Succeeded;
	}

	ACharacter* pPlayer = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if ( !IsValid(pPlayer) )
	{
		return EBTNodeResult::Failed;
	}

	EGreaterSpider_STATE AtkState = EGreaterSpider_STATE::DEFAULT;

	if ( _OwnComp.GetBlackboardComponent()->GetValueAsBool(FName("Phase2")) )
	{
		if ( _OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("Phase2Pattern")) != 1 )
		{
			FVector LookVector = pPlayer->GetActorLocation() - pBoss->GetActorLocation();
			LookVector.Z = 0.f;
			FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
			pBoss->SetActorRotation(TargetRot);

			if (_OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("Phase2Pattern")) == 2)
			{
				AtkState = EGreaterSpider_STATE::RANGEATTACK;
			}
			else
			{
				AtkState = EGreaterSpider_STATE::RUSHATTACK;
			}
		}
		else
		{
			int32 fAttackPattern = _OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("PatternNumber"));

			// 오른쪽 공격
			if (fAttackPattern == 1)
			{
				AtkState = EGreaterSpider_STATE::RIGHTATTACK;
			}
			// 왼쪽 공격
			else if (fAttackPattern == 2)
			{
				AtkState = EGreaterSpider_STATE::LEFTATTACK;
			}
			// 정면 공격
			else if (fAttackPattern == 0)
			{
				AtkState = EGreaterSpider_STATE::CENTERATTACK;
			}
		}
	}
	else
	{
		int32 fAttackPattern = _OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("PatternNumber"));

		// 오른쪽 공격
		if (fAttackPattern == 1)
		{
			AtkState = EGreaterSpider_STATE::RIGHTATTACK;
		}
		// 왼쪽 공격
		else if (fAttackPattern == 2)
		{
			AtkState = EGreaterSpider_STATE::LEFTATTACK;
		}
		// 정면 공격
		else if (fAttackPattern == 0)
		{
			AtkState = EGreaterSpider_STATE::CENTERATTACK;
		}
	}
	pBoss->PlayGSMontage(AtkState);

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

	if (!AnimInst->IsAnyMontagePlaying())
	{
		// idle 상태로
		pBoss->SetiTurnDir(0);
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
	
}
