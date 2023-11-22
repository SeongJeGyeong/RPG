// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_NextPos_Monster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "../Monster_Base.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_NextPos_Monster::UBTT_NextPos_Monster()
{
	m_CenterPosKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_NextPos_Monster, m_CenterPosKey));
	m_NextPosKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_NextPos_Monster, m_NextPosKey));
}

EBTNodeResult::Type UBTT_NextPos_Monster::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	if (m_CenterPosKey.IsNone())
	{
		return EBTNodeResult::Failed;
	}

	if (_OwnComp.GetBlackboardComponent()->GetValueAsBool(FName("RecentPosSet")))
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("RecentPosSet"), false);
	}

	FVector vInitPos = _OwnComp.GetBlackboardComponent()->GetValueAsVector(m_CenterPosKey.SelectedKeyName);

	FNavLocation nLocation;
	UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	pNavSys->GetRandomPointInNavigableRadius(vInitPos, m_RangeFromCenter, nLocation);

	FVector vNextPos = nLocation.Location;

	if (m_NextPosKey.IsNone())
	{
		return EBTNodeResult::Failed;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());
	if (IsValid(pMonster))
	{
		pMonster->ChangeState(EMONSTER_STATE::WALK);
		pMonster->GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	_OwnComp.GetBlackboardComponent()->SetValueAsVector(m_NextPosKey.SelectedKeyName, vNextPos);

	return EBTNodeResult::Succeeded;
}
