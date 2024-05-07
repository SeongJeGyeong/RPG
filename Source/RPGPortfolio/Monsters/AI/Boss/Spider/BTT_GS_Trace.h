// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GS_Trace.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_GS_Trace : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = ( AllowPrivateAccess = true ))
	FBlackboardKeySelector m_TargetKey;

public:
	UBTT_GS_Trace();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds) override;
};
