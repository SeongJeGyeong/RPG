// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Boss_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_Boss_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class ABoss_Base* m_Boss;

	float fRotateRate;

public:
	UBTT_Boss_TurnToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds) override;
};
