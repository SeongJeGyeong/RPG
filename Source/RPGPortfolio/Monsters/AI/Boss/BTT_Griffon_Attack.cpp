// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Attack.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../MonsterAnim/AnimInstance_Boss_Base.h"
#include "Animation/AnimSequenceBase.h"

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

	m_AnimInst = Cast<UAnimInstance_Boss_Base>(pMonster->GetMesh()->GetAnimInstance());

	switch (PatternNum)
	{
	case 1:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO1);
		m_AnimSeq = LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/QuadrapedCreatures/Griffon/Animations/ANIM_Griffon_RightClawsAttack.ANIM_Griffon_RightClawsAttack'"));
		break;
	case 2:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO2);
		m_AnimSeq = LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/QuadrapedCreatures/Griffon/Animations/ANIM_Griffon_2HitComboAttack.ANIM_Griffon_2HitComboAttack'"));
		break;
	case 3:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO3);
		m_AnimSeq = LoadObject<UAnimSequenceBase>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/QuadrapedCreatures/Griffon/Animations/ANIM_Griffon_3HitComboAttack.ANIM_Griffon_3HitComboAttack'"));
		break;
	default:
		break;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Griffon_Attack::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (!m_AnimInst->IsPlayingSlotAnimation(m_AnimSeq, FName("Attack")))
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimEnded"));
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
}
