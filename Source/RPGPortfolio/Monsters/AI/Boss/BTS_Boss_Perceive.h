// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Boss_Perceive.generated.h"

/**
 *  사용안함
 */
UCLASS()
class RPGPORTFOLIO_API UBTS_Boss_Perceive : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_Boss_Perceive();
	~UBTS_Boss_Perceive();

public:
	virtual void TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT) override;

public:
	bool bDetect;
};
