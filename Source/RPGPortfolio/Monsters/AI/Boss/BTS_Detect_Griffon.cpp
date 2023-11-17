// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Detect_Griffon.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_Detect_Griffon::UBTS_Detect_Griffon()
	: bDetect(false)
	, bIsAtkRange(false)
{
	NodeName = TEXT("Detect Player");
	Interval = 0.5f;
}

UBTS_Detect_Griffon::~UBTS_Detect_Griffon()
{
}

void UBTS_Detect_Griffon::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
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

	ACharacter* pTarget = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if (!IsValid(pTarget))
	{
		return;
	}
	
	float Distance = FVector::Distance(pMonster->GetActorLocation(), pTarget->GetActorLocation());

	if (Distance < pMonster->GetMonsterInfo().DetectRange)
	{
		pController->GetBlackboardComponent()->SetValueAsInt(TEXT("InnerChk"), 1);
		bDetect = true;
	}
	else
	{
		pController->GetBlackboardComponent()->SetValueAsInt(TEXT("InnerChk"), 0);
		bDetect = false;
	}

	FColor color;
	bDetect ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().DetectRange, 40, color, false, 0.4f);
}
