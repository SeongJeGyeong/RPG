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
	UPROPERTY()
	bool bToggleLockOn;

	// 록온 실패시 카메라 정면 회전값
	FRotator rForwardRotation;

	UPROPERTY()
	class APlayer_Base_Knight* m_Player;

	// 록온 타겟 컴포넌트
	UPROPERTY(BlueprintReadOnly)
	class ULockOnTargetComponent* m_Target;

	// 락온 실패시 시점 초기화 타이머
	FTimerDelegate LockOnFailedDelegate;	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool ToggleCameraLockOn(const bool& _ToggleLockOn);
	void LockOnTarget(ULockOnTargetComponent* NewTargetComponent);
	void BreakLockOnTarget();
	class ULockOnTargetComponent* GetLockTarget();
	void SwitchTarget(ELockOnDirection SwitchDirection);
	TArray<class ULockOnTargetComponent*> GetTargetComponents();

	void ResetCamera();

	/* True if the camera is currently locked to a target */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Lock On Camera")
	bool IsCameraLockedToTarget();

};
