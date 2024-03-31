// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_OnBeingHit.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTD_OnBeingHit : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = ( AllowPrivateAccess = true ))
	FBlackboardKeySelector m_WasHit;

public:
	UBTD_OnBeingHit();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) const override;
};
