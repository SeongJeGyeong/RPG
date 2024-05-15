// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "LockOnTargetComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class RPGPORTFOLIO_API ULockOnTargetComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	ULockOnTargetComponent();

	void SetLockOnMarkVisibility(bool _Visible);

	void SetLockOn(bool _LockOn);
	bool IsOwnerDead();	
};
