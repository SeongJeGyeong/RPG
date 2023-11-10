// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Monster_Base.h"
#include "../Monsters_Base.h"
#include "../Monster_Base.h"

void AAIC_Monster_Base::OnPossess(APawn* _Owner)
{
	Super::OnPossess(_Owner);

	//AMonsters_Base* pMonster = Cast<AMonsters_Base>(_Owner);
	AMonster_Base* pMonster = Cast<AMonster_Base>(_Owner);

	if (!IsValid(pMonster))
	{
		UE_LOG(LogTemp, Error, TEXT("몬스터 AI 빙의 실패"));
		return;
	}

	m_BehaviorTree = pMonster->GetBehaviorTree();
	m_Blackboard = pMonster->GetBlackboard();

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

void AAIC_Monster_Base::OnUnPossess()
{
	Super::OnUnPossess();
}
