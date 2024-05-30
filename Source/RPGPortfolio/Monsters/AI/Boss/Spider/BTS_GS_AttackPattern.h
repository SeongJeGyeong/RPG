// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_GS_AttackPattern.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTS_GS_AttackPattern : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_GS_AttackPattern();
	
	// 노드에 연관성이 생길 때 호출되는 함수
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
};
