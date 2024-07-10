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

	// 락온 실패시 시점 초기화 타이머
	FTimerDelegate LockOnFailedDelegate;
	FTimerHandle LockOnFailedTimer;

public:
	// 록온 토글 (캐릭터 애님 블루프린트에서 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bToggleLockOn;

	// 록온 실패시 카메라 정면 회전값
	FRotator rForwardRotation;

	// 록온 타겟 컴포넌트
	UPROPERTY()
	class ULockOnTargetComponent* m_Target;

protected:
	virtual void BeginPlay() override;

public:
	bool ToggleCameraLockOn(const bool& _ToggleLockOn);
	void BreakLockOnTarget();
	void SwitchTarget(ELockOnDirection SwitchDirection);

	bool IsCameraLockedToTarget();

private:
	void LockOnTarget(ULockOnTargetComponent* NewTargetComponent);
	TArray<class ULockOnTargetComponent*> GetTargetComponents();
	class ULockOnTargetComponent* GetLockTarget();

	void ResetCamera();
};
