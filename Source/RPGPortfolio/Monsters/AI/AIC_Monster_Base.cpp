// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Monster_Base.h"
#include "../Monster_Base.h"
#include "behaviortree/BehaviorTree.h"
#include "behaviortree/blackboarddata.h"
#include "behaviortree/blackboardcomponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

const FName AAIC_Monster_Base::PatternNumber(TEXT("PatternKey"));

AAIC_Monster_Base::AAIC_Monster_Base()
{
	m_AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));
	m_AISight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

	m_AISight->SightRadius = 800.f;					// 시야 반경
	m_AISight->LoseSightRadius = 1600.f;			// 포착한 대상을 상실하는 시야 반경
	m_AISight->PeripheralVisionAngleDegrees = 45.f;	// 시야각 : 설정치수로 각각 왼쪽, 오른쪽의 시야각을 설정. 45의 경우 총 90도의 시야각을 가짐
	m_AISight->SetMaxAge(2.f);
	m_AISight->DetectionByAffiliation.bDetectEnemies = true;

	// 감지설정 객체 등록
	m_AIPerception->ConfigureSense(*m_AISight);
	// 우선순위 등록
	m_AIPerception->SetDominantSense(m_AISight->GetSenseImplementation());
}

void AAIC_Monster_Base::BeginPlay()
{
	Super::BeginPlay();

	m_AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAIC_Monster_Base::PerceptionUpdate);
}

void AAIC_Monster_Base::OnPossess(APawn* _Owner)
{
	Super::OnPossess(_Owner);

	AMonster_Base* pMonster = Cast<AMonster_Base>(_Owner);

	if (!IsValid(pMonster))
	{
		UE_LOG(LogTemp, Error, TEXT("몬스터 AI 빙의 실패"));
		return;
	}

	if (pMonster->GetBehaviorTree())
	{
		m_BehaviorTree = pMonster->GetBehaviorTree();
	}

	if (pMonster->GetBlackboard())
	{
		m_Blackboard = pMonster->GetBlackboard();
	}

	// 몬스터 팀 설정(1)
	SetGenericTeamId(1);

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

float AAIC_Monster_Base::GetLoseSightRadius() const
{
	return m_AISight->LoseSightRadius;
}

void AAIC_Monster_Base::PerceptionUpdate(const TArray<AActor*>& _UpdateActors)
{
	// 인자로 받은 액터배열은 LoseSightRadius를 벗어날 때에도 타겟 정보를 가져오기 때문에 타겟이 들어왔을때와 나갔을 때를 분간하기 어렵다
	// GetCurrentlyPerceivedActors 함수를 사용해 시야 안에 들어올 때만 액터를 가져옴
	TArray<AActor*> PerceivedActors;
	m_AIPerception->GetCurrentlyPerceivedActors(nullptr, PerceivedActors);

	// 시야에 들어오면 타겟을 세팅하고 타겟 추적 범위에서 벗어났을 때 세팅을 해제한다
	if (PerceivedActors.Num() > 0)
	{
		GetBlackboardComponent()->SetValueAsObject("Target", PerceivedActors[0]);
	}
}
