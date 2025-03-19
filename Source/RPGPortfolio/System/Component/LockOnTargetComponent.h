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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = "true" ))
	class UWidgetComponent* m_LockOnMark;

public:
	ULockOnTargetComponent();

	void SetLockOn(bool _LockOn);
	bool IsOwnerDead();	
};
