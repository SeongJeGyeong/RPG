// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_CheckRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "../../Monster_Base.h"

UBTT_Griffon_CheckRange::UBTT_Griffon_CheckRange()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Griffon_CheckRange, m_TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_Griffon_CheckRange::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTT_Griffon_CheckRange::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (m_TargetKey.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	ACharacter* pCharacter = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(m_TargetKey.SelectedKeyName));
	if (!IsValid(pCharacter))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if (nullptr == pMonster)
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	float Distance = FVector::Distance(pMonster->GetActorLocation(), pCharacter->GetActorLocation());

	if (Distance < pMonster->GetMonsterInfo().DetectRange)
	{
		bInnerRange = true;
		pController->GetBlackboardComponent()->SetValueAsInt(TEXT("InnerChk"), 1);
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}
	else
	{
		bInnerRange = false;
		pController->GetBlackboardComponent()->SetValueAsInt(TEXT("InnerChk"), 0);
	}

	/*FColor color;
	bInnerRange ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().DetectRange, 40, color, false, 0.4f);*/
}
