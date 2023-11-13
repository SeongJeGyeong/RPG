// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Griffon_Perceive.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_Griffon_Perceive::UBTS_Griffon_Perceive()
{
}

UBTS_Griffon_Perceive::~UBTS_Griffon_Perceive()
{
}

void UBTS_Griffon_Perceive::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
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

	if (Distance < pMonster->GetMonsterInfo().BOSS_PerceiveRange)
	{
		pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), pPlayer);
	}
	else
	{
		pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}

#ifdef ENABLE_DRAW_DEBUG
	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().BOSS_PerceiveRange, 40, FColor::Emerald, false, 0.4f);
#endif
}
