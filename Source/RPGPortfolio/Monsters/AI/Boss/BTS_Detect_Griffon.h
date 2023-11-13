// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Detect_Griffon.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTS_Detect_Griffon : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_Detect_Griffon();
	~UBTS_Detect_Griffon();

public:
	virtual void TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT) override;

public:
	bool bDetect;
	bool bIsAtkRange;

};
