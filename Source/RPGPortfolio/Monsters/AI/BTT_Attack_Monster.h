// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack_Monster.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_Attack_Monster : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_Attack_Monster();

public:
	// Task 진입 시 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Task 가 InProgress 상태면 매 틱마다 호출
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
