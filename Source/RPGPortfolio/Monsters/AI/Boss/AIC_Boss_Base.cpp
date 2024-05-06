// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Boss_Base.h"
#include "behaviortree/BehaviorTree.h"
#include "behaviortree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Boss_Base.h"

void AAIC_Boss_Base::OnPossess(APawn* _Owner)
{
	Super::OnPossess(_Owner);

	ABoss_Base* pBoss = Cast<ABoss_Base>(_Owner);

	if (!IsValid(pBoss))
	{
		UE_LOG(LogTemp, Error, TEXT("몬스터 AI 빙의 실패"));
		return;
	}

	if (pBoss->GetBehaviorTree())
	{
		m_BehaviorTree = pBoss->GetBehaviorTree();
	}

	if (pBoss->GetBlackboard())
	{
		m_Blackboard = pBoss->GetBlackboard();
	}

	if (IsValid(m_Blackboard))
	{
		UBlackboardComponent* pCom = nullptr;
		UseBlackboard(m_Blackboard, pCom);

		if (IsValid(m_BehaviorTree))
		{
			RunBehaviorTree(m_BehaviorTree);
		}
	}
}

void AAIC_Boss_Base::OnUnPossess()
{
	Super::OnUnPossess();
}
