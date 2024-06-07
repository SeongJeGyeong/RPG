// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericTeamAgentInterface.h"
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
class RPGPORTFOLIO_API APlayer_Base_Knight : public ACharacter, public IGenericTeamAgentInterface
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

	// 락온 표시용
	UPROPERTY()
	TSubclassOf<class UUserWidget> m_MarkerClass;
	UPROPERTY()
	class UUserWidget* m_Marker;

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

	UPROPERTY()
	TArray<AActor*> HitActorArr;

private:
	// Lock On
	float LockonControlRotationRate;
	bool bLockOn;
	FTimerDelegate LockOnDelegate;

	// 이동 가능한 상태인지 체크용
	bool bEnableMove;

	// 공격 체크용 토글
	bool bAttackToggle;
	// 강공격 체크용 토글
	bool bHeavyToggle;
	// 다음 공격 입력 체크용
	bool bNextAtkCheckOn;
	// 약공격인지 강공격인지 판별(true = 강, false = 약)
	bool bHeavyAtk;

	// 공격 모션 도중 움직이지 못하는 상태 체크
	bool bInvalidInput;

	bool bJumpAtk;
	bool bAtkTrace;
	bool bSprintToggle;

	// 아이템 사용 딜레이 체크용
	bool bItemDelay;

	// 공격 중 이동 체크용
	bool bAtkMove;
	FVector vAtkMoveVec;
	FRotator rAtkMoveRot;

	// 구르기 관련
	bool bDodging;
	bool bDodgeMove;
	FVector vDodgeVector;
	FRotator rDodgeRotation;

	// 무적시간
	bool bToggleInvinc;
	float fInvincTime;

	// 방패 방어 상태
	bool bToggleGuard;

	// 공격 관련
	int32 CurrentCombo;

	bool bShowMenu;
	
	// 점프공격 타이머
	FTimerDelegate JumpAtkDelegate;
	// 방어 표현 타이머
	FTimerHandle BlockReactTimer;

public:
	void SetbAtkTrace(const bool& _AtkTrace) { bAtkTrace = _AtkTrace;}
	void SetbAtkToggle(const bool& _AtkToggle) { bAttackToggle = _AtkToggle; }
	void SetbNextAtkCheck(const bool& _NextAtkCheck) { bNextAtkCheckOn = _NextAtkCheck; }
	// 조작 불가 상태
	void SetbInvalidInput(const bool& _InvalidInput) { bInvalidInput = _InvalidInput; }
	// 이동 가능 상태
	void SetbEnableMove(const bool& _EnableMove) { bEnableMove = _EnableMove; }
	// 아이템 사용 딜레이
	bool GetbItemDelay() const { return bItemDelay; }
	void SetbItemDelay(const bool& _ItemDelay) { bItemDelay = _ItemDelay; }
	// 무적상태
	bool GetbInvincible() const { return bToggleInvinc; }
	// 가드상태
	bool GetbToggleGuard() const { return bToggleGuard; }
	void SetbToggleGuard(const bool& _ToggleGuard) { bToggleGuard = _ToggleGuard; }

	// 락온 중 플레이어가 적을 바라보고 있도록 설정
	void SetOrientRotation(const bool& _Val);
	// 락온 토글 상태 확인
	bool GetbToggleLockOn() const { return bLockOn; }
	// 회피 애니메이션 종료 설정
	void SetbDodging(const bool& _Dodging) { bDodging = _Dodging; }

	const UCameraComponent* GetCamera() { return m_Camera; }
	const UPlayer_CameraArm* GetArm() { return m_Arm; }

	void GainMonsterSoul(int32 _GainedSoul);

	void EmptyHitActorArr() { HitActorArr.Empty(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType, EPlayerMontage _AtkMontage);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	void AttackHitCheck(EATTACK_TYPE _AtkType);	// 어택 트레이스용
	void NextAttackCheck();	// 다음 공격 발동 체크
	void CloseMenuUI();	
	void DodgeTimeCheck(bool _Dodge); // 회피 무적시간 체크
	void AttackMoveStart(bool _AtkMove); // 공격 모션 중 이동
	bool BlockEnemyAttack(float _Damage, FVector _MonDir); // 적 공격 방어
	void UseItem(EITEM_ID _ID, EEQUIP_SLOT _Slot);
	void ConsumeStaminaForMontage(EPlayerMontage _Montage); // 애니메이션별 스태미나 소비
	void StopBlockPhysics(); // 적 공격 방어시 피직스 효과
	void JumpAttack();

	UFUNCTION()
	void TargetLockOn();
	UFUNCTION()
	void ItemDelaytime(float _DelayPercent);

	void BreakLockOn();
	void ShotProjectile();

	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(0); };	// 플레이어 팀 설정(0)

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
	void LockOnToggleAction(const FInputActionInstance& _Instance);
	void SwitchLockOnTarget(const FInputActionInstance& _Instance);
	void OpenMenu(const FInputActionInstance& _Instance);
	void ActionCommand(const FInputActionInstance& _Instance);
	void QuickSlotChange(const FInputActionInstance& _Instance);
	void UseLowerQuickSlot(const FInputActionInstance& _Instance);
	void UseSkill_1(const FInputActionInstance& _Instance);

	bool CheckMontagePlaying();

private:
	UFUNCTION()
	void ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	void ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index);


};
