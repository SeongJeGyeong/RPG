// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player_CameraArm.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayer_CameraArm : public USpringArmComponent
{
	GENERATED_BODY()
	
public:
	UPlayer_CameraArm();

public:
	/* Max Distance from the character for an actor to be targetable */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float MaxTargetLockDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	bool bUseSoftLock;

	/* Turn debug visuals on/off */
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	bool bDrawDebug;

	/* True if lock was recently broken and mouse delta is still high */
	bool bSoftlockRequiresReset;

	/* The component the camera is currently locked on to */
	UPROPERTY(BlueprintReadOnly)
	class ULockOnTargetComponent* CameraTarget;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleCameraLock();
	void ToggleSoftLock();
	void LockToTarget(ULockOnTargetComponent* NewTargetComponent);
	void BreakTargetLock();
	class ULockOnTargetComponent* GetLockTarget();
	void SwitchTarget(ELockOnDirection SwitchDirection);
	TArray<class ULockOnTargetComponent*> GetTargetComponents();

	/* True if the camera is currently locked to a target */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lock On Camera")
	bool IsCameraLockedToTarget();

};
