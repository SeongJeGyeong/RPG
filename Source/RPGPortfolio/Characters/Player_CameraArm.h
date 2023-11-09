// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputMappingContext.h"
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
	// 록온 가능 거리
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	float fMaxTargetLockDistance;

	// 디버그 온오프
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera")
	bool bDrawDebug;

	// 록온 토글
	bool bToggleLockOn;

	// 록온 실패시 카메라 정면 회전값
	FRotator rForwardRotation;

	class APlayer_Base_Knight* m_Player;

	// 록온 타겟 컴포넌트
	UPROPERTY(BlueprintReadOnly)
	class ULockOnTargetComponent* m_Target;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ToggleCameraLockOn(const FInputActionInstance& _Instance);
	void LockOnTarget(ULockOnTargetComponent* NewTargetComponent);
	void BreakLockOnTarget();
	class ULockOnTargetComponent* GetLockTarget();
	void SwitchTarget(ELockOnDirection SwitchDirection);
	TArray<class ULockOnTargetComponent*> GetTargetComponents();

	/* True if the camera is currently locked to a target */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lock On Camera")
	bool IsCameraLockedToTarget();

};
