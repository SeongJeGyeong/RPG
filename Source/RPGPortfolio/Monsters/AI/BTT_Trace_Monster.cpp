// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Trace_Monster.h"
#include "AIController.h"
#include "../Monster_Base.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_Trace_Monster::UBTT_Trace_Monster()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Trace_Monster, m_TargetKey), AActor::StaticClass());
	m_RecentTargetPos.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Trace_Monster, m_RecentTargetPos));

	m_RevaluateRange = 100.f;
}

EBTNodeResult::Type UBTT_Trace_Monster::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	if (m_TargetKey.IsNone() || m_RecentTargetPos.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));
		return EBTNodeResult::Succeeded;
	}

	AAIController* pController = _OwnComp.GetAIOwner();

	ACharacter* pCharacter = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(m_TargetKey.SelectedKeyName));

	if (!IsValid(pCharacter))
	{
		return EBTNodeResult::Succeeded;
	}

	pController->MoveToLocation(pCharacter->GetActorLocation());
	_OwnComp.GetBlackboardComponent()->SetValueAsVector(m_RecentTargetPos.SelectedKeyName, pCharacter->GetActorLocation());
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	pMonster->GetCharacterMovement()->MaxWalkSpeed = 400.f;

	return EBTNodeResult::InProgress;
}

void UBTT_Trace_Monster::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (m_TargetKey.IsNone() || m_RecentTargetPos.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));
		
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	ACharacter* pTarget = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if (!IsValid(pTarget))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	float fAtkRange = _OwnComp.GetBlackboardComponent()->GetValueAsFloat(FName("AtkRange"));
	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());

	if (!IsValid(pMonster))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	float Distance = FVector::Distance(pTarget->GetActorLocation(), pMonster->GetActorLocation());

	if (Distance < fAtkRange - 50.f)
	{
		pController->StopMovement();
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	FVector vRecentDest = _OwnComp.GetBlackboardComponent()->GetValueAsVector(m_RecentTargetPos.SelectedKeyName);

	// 타겟의 최근 위치와 타겟의 현재 위치가 1m 이상 차이나면 최근 위치를 재설정
	if (m_RevaluateRange < FVector::Distance(pTarget->GetActorLocation(), vRecentDest))
	{
		pController->MoveToLocation(pTarget->GetActorLocation());

		_OwnComp.GetBlackboardComponent()->SetValueAsVector(m_RecentTargetPos.SelectedKeyName, pTarget->GetActorLocation());
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("RecentPosSet"), true);
	}
}
