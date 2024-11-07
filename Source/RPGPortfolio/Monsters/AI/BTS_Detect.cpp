// Fill out your copyright notice in the Description page of Project Settings.



#include "BTS_Detect.h"
#include "AIC_Monster_Base.h"
#include "../Monster_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_Detect::UBTS_Detect()
	: bDetect(false)
	, bIsAtkRange(false)
{
	// 감지된 플레이어가 탐지범위 내에 있는지 확인
	NodeName = TEXT("Detect Player");
	Interval = 0.5f;
}

UBTS_Detect::~UBTS_Detect()
{
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnComp, _NodeMemory, _DT);

	AAIC_Monster_Base* pController = Cast<AAIC_Monster_Base>(_OwnComp.GetAIOwner());
	if (!IsValid(pController))
	{
		return;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if (!IsValid(pMonster))
	{
		return;
	}

	ACharacter* pPlayer = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if ( !IsValid(pPlayer) )
	{
		return;
	}

	float Distance = FVector::Distance(pMonster->GetActorLocation(), pPlayer->GetActorLocation());
	if (Distance >= pController->GetLoseSightRadius())
	{
		pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
}
