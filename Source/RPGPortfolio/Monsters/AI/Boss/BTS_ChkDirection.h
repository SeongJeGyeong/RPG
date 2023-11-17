// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ChkDirection.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTS_ChkDirection : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_ChkDirection();
	~UBTS_ChkDirection();
	
public:
	virtual void TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT) override;

};
