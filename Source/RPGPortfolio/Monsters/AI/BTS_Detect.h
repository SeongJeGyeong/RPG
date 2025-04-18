// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Detect.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTS_Detect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_Detect();
	~UBTS_Detect();

public:
	virtual void TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT) override;

public:
	bool bDetect;
	bool bIsAtkRange;
};
