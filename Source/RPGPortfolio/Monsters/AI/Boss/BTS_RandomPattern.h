// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_RandomPattern.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTS_RandomPattern : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_RandomPattern();
	~UBTS_RandomPattern();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT) override;
};
