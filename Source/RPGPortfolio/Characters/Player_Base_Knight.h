// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../System/DataAsset/DA_MenuSound.h"
#include "../System/DataAsset/DA_PlayerSound.h"
#include "../System/DataAsset/DA_PlayerMontage.h"
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

	UPROPERTY(EditAnywhere, Category = "Animation", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UDA_PlayerMontage> m_PlayerMontage;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UDA_PlayerSound> m_PlayerSound;

	UPROPERTY(EditAnywhere, Category = "MenuSound", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UDA_MenuSound> m_MenuSound;

	UPROPERTY()
	UAnimInstance_Knight* m_AnimInst;
	UPROPERTY()
	TArray<TScriptInterface<class IPlayerInteraction>> OverlapInteractionArr;
	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_SettingAttackMontage;
	UPROPERTY()
	class UUI_Base* m_MainUI;
	UPROPERTY()
	class UUI_Player_Main* m_PlayerUI;

private:
	// 이동 블렌드스페이스용
	float fFrontBack;
	float fLeftRight;

	// Lock On
	float LockonControlRotationRate;

	// 점프, 이동 가능한 상태인지 체크용
	bool bEnableJump;
	bool bEnableMove;

	// 다음 공격 체크용 토글
	bool bAttackToggle;
	// 강공격 체크용 토글
	bool bHeavyToggle;
	// 다음 공격 입력 체크용
	bool bNextAtkCheckOn;

	bool bJumpAtk;
	bool bAtkTrace;
	bool bSprintToggle;

	// 아이템 사용 딜레이 체크용
	bool bItemDelay;
	float fItemDelayTime;

	// 아이템 사용 애니메이션 재생 중
	bool bItemInUse;

	// 공격 중 이동 체크용
	bool bAtkMove;
	FVector vAtkMoveVec;
	FRotator rAtkMoveRot;

	// 구르기 관련
	FVector vDodgeVector;
	FRotator rDodgeRotation;

	// 무적시간
	bool bToggleInvinc;
	float fInvincTime;

	// 방패 방어 상태
	bool bToggleGuard;

	// 공격 관련
	int32 CurrentCombo;
	int32 MaxCombo;

	bool bShowMenu;

	FTimerHandle BlockReactTimer;

public:
	float GetfFrontBack() const { return fFrontBack; }
	void SetfFrontBack(const float& _FrontBack) { fFrontBack = _FrontBack; }
	float GetfLeftRight() const { return fLeftRight; }
	void SetfLeftRight(const float& _LeftRight) { fLeftRight = _LeftRight; }
	bool GetbAtkTrace() const { return bAtkTrace; }
	void SetbAtkTrace(const bool& _AtkTrace) { bAtkTrace = _AtkTrace;}
	void SetbAtkToggle(const bool& _AtkToggle) { bAttackToggle = _AtkToggle; }
	bool GetbNextAtkCheck() const { return bNextAtkCheckOn; }
	void SetbNextAtkCheck(const bool& _NextAtkCheck) { bNextAtkCheckOn = _NextAtkCheck; }
	bool GetbEnableJump() const { return bEnableJump; }
	void SetbEnableJump(const bool& _EnableJump) { bEnableJump = _EnableJump; }
	bool GetbEnableMove() const { return bEnableMove; }
	void SetbEnableMove(const bool& _EnableMove) { bEnableMove = _EnableMove; }
	bool GetbItemDelay() const { return bItemDelay; }
	bool GetbInvincible() const { return bToggleInvinc; }
	void SetbInvincible(const bool& _ToggleInvinc) { bToggleInvinc = _ToggleInvinc; }
	bool GetbToggleGuard() const { return bToggleGuard; }
	void SetbToggleGuard(const bool& _ToggleGuard) { bToggleGuard = _ToggleGuard; }
	bool GetbSprintToggle() const { return bSprintToggle; }
	void SetOrientRotation(const bool& _Val);

	const UCameraComponent* GetCamera() { return m_Camera; }
	const UPlayer_CameraArm* GetArm() { return m_Arm; }

	const TSoftObjectPtr<UAnimMontage> GetAttackMontage() { return m_SettingAttackMontage; }
	void SetAttackMontage(TSoftObjectPtr<UAnimMontage> _SettingAttackMontage) { m_SettingAttackMontage = _SettingAttackMontage; }

	void GainMonsterSoul(int32 _GainedSoul);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	void AttackHitCheck(EATTACK_TYPE _AtkType);	// 어택 트레이스용
	void NextAttackCheck();
	void CloseMenuUI();
	void ItemUseDelayOn();
	void InvincibleTimeCheck(bool _Invincible);
	void AttackMoveStart(bool _AtkMove);
	bool BlockEnemyAttack(float _Damage, FVector _MonDir);
	void UseItem(FString _NiagaraPath);
	void ConsumeStaminaForMontage(EPlayerMontage _Montage);
	void StopBlockPhysics();
	void BreakLockOn();

private:
	void MoveAction(const FInputActionInstance& _Instance);
	void RotateAction(const FInputActionInstance& _Instance);
	void JumpAction(const FInputActionInstance& _Instance);
	void SprintToggleAction(const FInputActionInstance& _Instance);
	void GuardAction(const FInputActionInstance& _Instance);
	void AttackAction(const FInputActionInstance& _Instance);
	void HeavyAttackToggle(const FInputActionInstance& _Instance);
	void DodgeAction(const FInputActionInstance& _Instance);
	void ParryAction(const FInputActionInstance& _Instance);
	void SwitchLockOnTarget(const FInputActionInstance& _Instance);
	void OpenMenu(const FInputActionInstance& _Instance);
	void ActionCommand(const FInputActionInstance& _Instance);
	void BackToPrevMenu(const FInputActionInstance& _Instance);
	void QuickSlotChange(const FInputActionInstance& _Instance);
	void UseLowerQuickSlot(const FInputActionInstance& _Instance);

	bool CheckMontagePlaying();

private:
	UFUNCTION()
	void ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	void ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index);
};
