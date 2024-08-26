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

private:
	// 록온 가능 거리
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera", meta = ( AllowPrivateAccess = "true" ))
	float fMaxTargetLockDistance;

	// 디버그 온오프
	UPROPERTY(EditDefaultsOnly, Category = "Lock On Camera", meta = ( AllowPrivateAccess = "true" ))
	bool bDrawDebug;

	UPROPERTY()
	class APlayer_Base_Knight* m_Player;

public:
	// 록온 타겟 컴포넌트
	UPROPERTY()
	class ULockOnTargetComponent* m_Target = nullptr;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool ToggleCameraLockOn(const bool& _ToggleLockOn);
	void BreakLockOnTarget();
	void SwitchTarget(ELockOnDirection SwitchDirection);

	bool IsCameraLockedToTarget();

private:
	void LockOnTarget(ULockOnTargetComponent* NewTargetComponent);
	TArray<class ULockOnTargetComponent*> GetTargetComponents();
	class ULockOnTargetComponent* GetLockTarget();
};
