// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TurnToTarget_Monster.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_TurnToTarget_Monster : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_TurnToTarget_Monster();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
