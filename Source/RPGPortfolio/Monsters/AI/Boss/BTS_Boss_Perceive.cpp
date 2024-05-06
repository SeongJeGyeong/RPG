// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Boss_Perceive.h"
#include "AIController.h"
#include "../../Boss_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_Boss_Perceive::UBTS_Boss_Perceive()
{
}

UBTS_Boss_Perceive::~UBTS_Boss_Perceive()
{
}

void UBTS_Boss_Perceive::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnComp, _NodeMemory, _DT);

	AAIController* pController = _OwnComp.GetAIOwner();
	if (!IsValid(pController))
	{
		return;
	}

	ABoss_Base* pBoss = Cast<ABoss_Base>(pController->GetPawn());
	if (!IsValid(pBoss))
	{
		return;
	}

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);	
	if (!IsValid(pPlayer))
	{
		return;
	}

	pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), pPlayer);
}
