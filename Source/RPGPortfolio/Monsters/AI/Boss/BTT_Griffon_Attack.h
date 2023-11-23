// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Griffon_Attack.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UBTT_Griffon_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector m_TargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = true))
	class UAnim_Griffon* m_AnimInst;

	FVector vForward;
	float fForwardDist;
	class UCharacterMovementComponent* m_Movement;

public:
	UBTT_Griffon_Attack();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds) override;
};
