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
	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = ( AllowPrivateAccess = "true" ))
	float fMaxLockOnDistance;			// 록온 가능 거리

	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = ( AllowPrivateAccess = "true" ))
	float LockonControlRotationRate;	// 락온 중 타겟 방향으로의 회전보간 속도

	UPROPERTY(EditDefaultsOnly, Category = "LockOn", meta = ( AllowPrivateAccess = "true" ))
	bool bDrawDebug;					// 디버그 온오프

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

	bool IsLockedOn();

	class ULockOnTargetComponent* GetLockOnTarget() const { return m_Target; };

private:
	void LockOnTarget(ULockOnTargetComponent* NewTargetComponent);
	TArray<class ULockOnTargetComponent*> GetTargetComponents();
	class ULockOnTargetComponent* GetSelectedTarget();
};
