// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_UA_Atk_Melee.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_UA_Atk_Melee : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_UA_Atk_Melee();

public:
	// Task 진입 시 호출
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
