// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../State/StateMachine.h"
#include "InputMappingContext.h"
#include "../../System/DataAsset/DA_InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_InputComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnQSChangeDelegate);				// 퀵슬롯 변경 델리게이트
DECLARE_DELEGATE_RetVal_OneParam(float, FOnConsumeStaminaDelegate, EPlayerStateType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayer_InputComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Input", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UInputMappingContext> m_IMC;

	UPROPERTY(EditAnywhere, Category = "Input", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UDA_InputAction>	m_IA_Setting;

	TUniquePtr<StateMachine> CurrentState;

	UPROPERTY()
	APlayer_Base_Knight*	m_Player;

	bool bHeavyHold;		// 강공격 체크용

	bool bInputGuard;		// 가드 키 입력 상태
	float fGuardWeight;		// 가드모션 블렌딩 웨이트

public:
	FOnQSChangeDelegate OnQSChange;
	FOnConsumeStaminaDelegate OnConsumeStamina;

public:	
	// Sets default values for this component's properties
	UPlayer_InputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetbInputGuard(const bool& _InputGuard) { bInputGuard = _InputGuard; }
	float GetGuardWeight() const { return fGuardWeight; }
	void SetGuardWeight(const float& _GuardWeight) { fGuardWeight = _GuardWeight; }

	bool GetbHeavyHold() const { return bHeavyHold; }

	EPlayerStateType GetStateType() { return CurrentState->GetStateType(); }
	void SetState(EPlayerStateType _StateType);

public:
	void Init(UEnhancedInputComponent* _InputComp);
	void SetMappingContext(ULocalPlayer* _LocalPlayer);

	void MoveAction(const FInputActionInstance& _Instance);
	void RotateAction(const FInputActionInstance& _Instance);
	void JumpAction(const FInputActionInstance& _Instance);
	void SprintStart();
	void SprintEnd();
	void GuardAction(const FInputActionInstance& _Instance);
	void AttackAction(const FInputActionInstance& _Instance);
	void HeavyAttack(const FInputActionInstance& _Instance);
	void DodgeAction(const FInputActionInstance& _Instance);
	void LockOnToggleAction(const FInputActionInstance& _Instance);
	void SwitchLockOnTarget(const FInputActionInstance& _Instance);
	void OpenMenu(const FInputActionInstance& _Instance);
	void ActionCommand(const FInputActionInstance& _Instance);
	void QuickSlotChange(const FInputActionInstance& _Instance);
	void UseLowerQuickSlot(const FInputActionInstance& _Instance);
	void UseSkill_1(const FInputActionInstance& _Instance);

	bool IsPossibleStateTransition(EPlayerStateType _State);
};
