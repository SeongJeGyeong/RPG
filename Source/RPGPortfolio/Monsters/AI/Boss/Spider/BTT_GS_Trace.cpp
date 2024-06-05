// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GS_Trace.h"
#include "../../../Boss_GreaterSpider.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTT_GS_Trace::UBTT_GS_Trace()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_GS_Trace, m_TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_GS_Trace::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTT_GS_Trace::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if ( m_TargetKey.IsNone() )
	{
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));

		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	AAIController* pController = _OwnComp.GetAIOwner();
	ABoss_GreaterSpider* pBoss = Cast<ABoss_GreaterSpider>(pController->GetPawn());
	if ( !IsValid(pBoss) )
	{
		// InProgress 상태를 중단
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	ACharacter* pCharacter = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if ( !IsValid(pCharacter) )
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	// 몬스터와 타겟 사이의 거리
	float Distance = FVector::Distance(pCharacter->GetActorLocation(), pBoss->GetActorLocation());

	float fAtkRange = _OwnComp.GetBlackboardComponent()->GetValueAsFloat(FName("AtkRange"));

	// 2페이즈 패턴을 사용할 경우
	if (_OwnComp.GetBlackboardComponent()->GetValueAsBool(FName("Phase2")) && _OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("Phase2Pattern")) != 1)
	{
		fAtkRange = _OwnComp.GetBlackboardComponent()->GetValueAsFloat(FName("RangedAtkRange"));
	}

	if (Distance < fAtkRange)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("InAtkRange"), true);
		pController->StopMovement();
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
	else
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(pController, pCharacter->GetActorLocation());
	}
}
