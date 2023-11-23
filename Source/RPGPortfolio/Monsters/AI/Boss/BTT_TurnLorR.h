// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TurnLorR.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_TurnLorR : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_TurnLorR();

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	class UAnim_Griffon* m_AnimInst;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds) override;
};
