// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "LockOnTargetComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class RPGPORTFOLIO_API ULockOnTargetComponent : public USphereComponent
{
	GENERATED_BODY()

private:
	bool bInvalidLockOn;

public:
	ULockOnTargetComponent();

	void SetLockOn(bool _LockOn);
	bool IsOwnerDead();
};
