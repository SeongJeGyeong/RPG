// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "behaviortree/BehaviorTree.h"
#include "behaviortree/blackboarddata.h"
#include "behaviortree/blackboardcomponent.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Monster_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API AAIC_Monster_Base : public AAIController
{
	GENERATED_BODY()
	
private:
	UBehaviorTree* m_BehaviorTree;
	UBlackboardData* m_Blackboard;

public:
	virtual void OnPossess(APawn* _Owner) override;
	virtual void OnUnPossess() override;

};
