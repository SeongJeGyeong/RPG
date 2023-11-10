// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Detect.h"
#include "AIController.h"
#include "../Monsters_Base.h"
#include "../Monster_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_Detect::UBTS_Detect()
	: bDetect(false)
	, bIsAtkRange(false)
{
	NodeName = TEXT("Detect Player");
	Interval = 0.5f;
}

UBTS_Detect::~UBTS_Detect()
{
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnComp, _NodeMemory, _DT);

	AAIController* pController = _OwnComp.GetAIOwner();

	if (!IsValid(pController))
	{
		return;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());

	if (!IsValid(pMonster))
	{
		return;
	}

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	float Distance = FVector::Distance(pMonster->GetActorLocation(), pPlayer->GetActorLocation());

	if (Distance < pMonster->GetMonsterInfo().DetectRange)
	{
		pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), pPlayer);
		bDetect = true;
	}
	else
	{
		pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		bDetect = false;
	}

	if (Distance < pMonster->GetMonsterInfo().AtkRange)
	{
		bIsAtkRange = true;
	}
	else
	{
		bIsAtkRange = true;
	}

#ifdef ENABLE_DRAW_DEBUG
	FColor color;
	bDetect ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().DetectRange, 40, color, false, 0.4f);
#endif
}
