// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_WithinRange_Monster.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTD_WithinRange_Monster : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	/*UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = true))
	float m_Range;*/

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector m_TargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector m_RangeKey;


public:
	UBTD_WithinRange_Monster();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) const override;

};
