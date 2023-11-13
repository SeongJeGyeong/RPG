// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Griffon_Perceive.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTS_Griffon_Perceive : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_Griffon_Perceive();
	~UBTS_Griffon_Perceive();

public:
	virtual void TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT) override;

public:
	bool bDetect;
	bool bIsAtkRange;


};
