// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GS_StartPhase2.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_GS_StartPhase2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_GS_StartPhase2();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds) override;
};
