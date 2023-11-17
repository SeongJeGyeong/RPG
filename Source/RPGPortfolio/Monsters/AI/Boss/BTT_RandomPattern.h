// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RandomPattern.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_RandomPattern : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RandomPattern();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
};
