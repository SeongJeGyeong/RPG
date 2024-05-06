// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Boss_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API AAIC_Boss_Base : public AAIController
{
	GENERATED_BODY()
	
private:
	class UBehaviorTree* m_BehaviorTree;
	class UBlackboardData* m_Blackboard;

public:
	virtual void OnPossess(APawn* _Owner) override;
	virtual void OnUnPossess() override;

};
