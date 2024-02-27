// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../System/DataAsset/DA_InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player_Base_Knight.generated.h"

class UAnimInstance_Knight;
class UPlayer_CameraArm;

UCLASS()
class RPGPORTFOLIO_API APlayer_Base_Knight : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Base_Knight();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UPlayer_CameraArm* m_Arm;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> m_IMC;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDA_InputAction>	m_IA_Setting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UAnimInstance_Knight* m_AnimInst;

	TSoftObjectPtr<UAnimMontage> m_AttackMontage;
	TSoftObjectPtr<UAnimMontage> m_PrimaryAttackMontage;
	TSoftObjectPtr<UAnimMontage> m_SettingAttackMontage;

	TSoftObjectPtr<UAnimMontage> m_DodgeMontage;
	TSoftObjectPtr<UAnimMontage> m_DodgeBWMontage;
	TSoftObjectPtr<UAnimMontage> m_ParryMontage;

private:
	////////// Lock On ///////////////
	float LockonControlRotationRate;
	float BrokeLockAimingCooldown;
	//////////////////////////////////

	bool bEnableJump;
	bool bEnableMove;
	bool bAttackToggle;
	bool bAtkTrace;
	
	FVector vDodgeVector;
	FRotator rDodgeRotation;

	int32 CurrentCombo;
	int32 MaxCombo;
	int32 iBaseDamage;
	bool bShowMenu;

	class UUI_Base* m_MainUI;
	class UUI_Player_Main* m_PlayerUI;
	TArray<class AItem_Dropped_Base*> OverlapItemArr;

public:
	bool GetbAtkTrace() { return bAtkTrace; }
	void SetbAtkTrace(bool _AtkTrace) { bAtkTrace = _AtkTrace;}
	bool GetbEnableJump() { return bEnableJump; }
	void SetbEnableJump(bool _EnableJump) { bEnableJump = _EnableJump; }
	bool GetbEnableMove() { return bEnableMove; }
	void SetbEnableMove(bool _EnableMove) { bEnableMove = _EnableMove; }

	FVector GetPlayerForwardVector() { return GetActorForwardVector(); }
	void SetOrientRotation(bool _Val);

	UCameraComponent* GetCamera() { return m_Camera; }
	UPlayer_CameraArm* GetArm() { return m_Arm; }

	TSoftObjectPtr<UAnimMontage> GetAttackMontage() { return m_SettingAttackMontage; }
	void SetAttackMontage(TSoftObjectPtr<UAnimMontage> _SettingAttackMontage) { m_SettingAttackMontage = _SettingAttackMontage; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	void NextAttackCheck();
	void AttackHitCheck();

private:
	void MoveAction(const FInputActionInstance& _Instance);
	void RotateAction(const FInputActionInstance& _Instance);
	void JumpAction(const FInputActionInstance& _Instance);
	void SprintToggleAction(const FInputActionInstance& _Instance);
	void GuardAction(const FInputActionInstance& _Instance);
	void AttackAction(const FInputActionInstance& _Instance);
	void PrimaryAttackAction(const FInputActionInstance& _Instance);
	void DodgeAction(const FInputActionInstance& _Instance);
	void ParryAction(const FInputActionInstance& _Instance);
	void SwitchLockOnTarget(const FInputActionInstance& _Instance);
	void OpenMenu(const FInputActionInstance& _Instance);
	void ActionCommand(const FInputActionInstance& _Instance);
	void BackToPrevMenu(const FInputActionInstance& _Instance);
	void QuickSlotChange(const FInputActionInstance& _Instance);

	bool CheckMontagePlaying();

private:
	UFUNCTION()
	void ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	void ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index);
};
