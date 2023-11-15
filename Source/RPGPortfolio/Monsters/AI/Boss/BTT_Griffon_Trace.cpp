// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Trace.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTT_Griffon_Trace::UBTT_Griffon_Trace()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Griffon_Trace, m_TargetKey), AActor::StaticClass());
	m_RecentTargetPos.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Griffon_Trace, m_RecentTargetPos));
}

EBTNodeResult::Type UBTT_Griffon_Trace::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if (nullptr == pMonster)
	{
		//return EBTNodeResult::Succeeded;
		return EBTNodeResult::Failed;
	}
	pMonster->GetCharacterMovement()->MaxWalkSpeed = 600.f;

	ACharacter* pCharacter = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(m_TargetKey.SelectedKeyName));
	if (!IsValid(pCharacter))
	{
		//return EBTNodeResult::Succeeded;
		return EBTNodeResult::Failed;
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsObject(FName("TraceTarget"), pCharacter);
	}

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(pController, pCharacter->GetActorLocation());
	_OwnComp.GetBlackboardComponent()->SetValueAsVector(m_RecentTargetPos.SelectedKeyName, pCharacter->GetActorLocation());

	return EBTNodeResult::InProgress;
}

void UBTT_Griffon_Trace::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (m_TargetKey.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));

		//FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if (!IsValid(pMonster))
	{
		// InProgress 상태를 중단
		//FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	ACharacter* pCharacter = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("TraceTarget")));
	if (!IsValid(pCharacter))
	{
		//FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	// 몬스터와 타겟 사이의 거리
	float Distance = FVector::Distance(pCharacter->GetActorLocation(), pMonster->GetActorLocation());
	float fAtkRange = _OwnComp.GetBlackboardComponent()->GetValueAsFloat(FName("AtkRange"));

	if (Distance < fAtkRange)
	{
		//FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
	else
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pController, pCharacter->GetActorLocation());
	}

	//FVector vRecentDest = _OwnComp.GetBlackboardComponent()->GetValueAsVector(m_RecentTargetPos.SelectedKeyName);

	//// 현재 타겟의 위치가 몬스터의 이동 목적지와 재평가거리 이상 멀어지면
	//if (m_RevaluateRange < FVector::Distance(pCharacter->GetActorLocation(), vRecentDest))
	//{
	//	// 현재 타겟의 위치로 다시 길찾기를 시도한다.
	//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(pController, pTarget->GetActorLocation());

	//	// 현재 추적대상의 위치(최근 이동 목적지)를 블랙보드에 기록
	//	OwnerComp.GetBlackboardComponent()->SetValueAsVector(m_RecentTargetPos.SelectedKeyName, pTarget->GetActorLocation());
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("RecentPosSet"), true);

	//}
}
