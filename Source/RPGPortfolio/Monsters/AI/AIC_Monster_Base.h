// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_Monster_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API AAIC_Monster_Base : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIC_Monster_Base();

private:
	UPROPERTY()
	class UBehaviorTree* m_BehaviorTree;
	UPROPERTY()
	class UBlackboardData* m_Blackboard;
	UPROPERTY()
	class UAIPerceptionComponent* m_AIPerception;
	UPROPERTY()
	class UAISenseConfig_Sight* m_AISight;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnPossess(APawn* _Owner) override;
	virtual void OnUnPossess() override;

	float GetLoseSightRadius() const;

	UFUNCTION()
	void PerceptionUpdate(const TArray<AActor*>& _UpdateActors);

	// UFUNCTION()
	// void OnTargetPerceived(AActor* _Actor, FAIStimulus const _Stimulus);

	static const FName PatternNumber;

	void SetAISightRadius(const float& _Radius);
	void SetAILoseSightRadius(const float& _Radius);
	void SetAIVisionAngle(const float& _Degree);
	void SaveAIConfigureSense();
};
