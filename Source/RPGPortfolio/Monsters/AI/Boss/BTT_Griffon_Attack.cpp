// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Attack.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../MonsterAnim/AnimInstance_Boss_Base.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_Griffon_Attack::UBTT_Griffon_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Griffon_Attack::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	int32 PatternNum = _OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("PatternKey"));

	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());
	if (nullptr == pMonster)
	{
		return EBTNodeResult::Succeeded;
	}
	m_Movement = pMonster->GetCharacterMovement();

	m_AnimInst = Cast<UAnimInstance_Boss_Base>(pMonster->GetMesh()->GetAnimInstance());
	if (!IsValid(m_AnimInst))
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
	vForward = pMonster->GetActorForwardVector();
	m_AnimInst->PlayAttackMontage(pMonster->GetBossState());

	return EBTNodeResult::InProgress;
}

void UBTT_Griffon_Attack::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (m_AnimInst->bIsAtkMove)
	{
		m_Movement->AddInputVector(vForward * 50.f * _DeltaSeconds);
	}

	if (!m_AnimInst->bIsAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimEnded"));
		/*AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());
		if (nullptr == pMonster)
		{
			FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		}
		pMonster->ChangeBossState(EBOSS_STATE::DEFAULT);*/
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
}
