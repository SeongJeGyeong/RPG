// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Monster_Hitted.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_Monster_Hitted : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_Monster_Hitted();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
