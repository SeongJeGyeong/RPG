// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_LoseTarget_Monster.h"
#include "../Monster_Base.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTT_LoseTarget_Monster::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());

	if (IsValid(pMonster))
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("RecentPosSet"), false);
		pMonster->GetCharacterMovement()->MaxWalkSpeed = 250.f;
	}

	return Super::ExecuteTask(_OwnComp, _NodeMemory);
}
