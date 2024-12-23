// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Trace_Monster.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_Trace_Monster : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector m_TargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector m_RecentTargetPos;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	float				   m_RevaluateRange;

public:
	UBTT_Trace_Monster();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds) override;
};
