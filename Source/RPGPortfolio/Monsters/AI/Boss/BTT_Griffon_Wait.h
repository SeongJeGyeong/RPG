// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTT_Griffon_Wait.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_Griffon_Wait : public UBTTask_Wait
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
};
