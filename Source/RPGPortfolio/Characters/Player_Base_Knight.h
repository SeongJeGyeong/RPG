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
	UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UPlayer_CameraArm* m_SArm;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UInputMappingContext> m_IMC;

	UPROPERTY(EditAnywhere, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDA_InputAction>	m_IA_Setting;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = ( AllowPrivateAccess = "true" ))
	UDA_PlayerMontage* m_PlayerMontage;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = ( AllowPrivateAccess = "true" ))
	UDA_PlayerSound* m_PlayerSound;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = ( AllowPrivateAccess = "true" ))
	UDA_MenuSound* m_MenuSound;

	UPROPERTY()
	TSubclassOf<class AProjectile_Base> m_Proj;

	UPROPERTY()
	UAnimInstance_Knight* m_AnimInst;
	UPROPERTY()
	TArray<TScriptInterface<class IPlayerInteraction>> OverlapInteractionArr;

	UPROPERTY()
	class UUI_Base* m_MainUI;
	UPROPERTY()
	class UUI_Player_Main* m_PlayerUI;

	UPROPERTY()
	TArray<AActor*> HitActorArr;

private:
	float LockonControlRotationRate;		// 락온 중 타겟 방향으로의 회전보간 속도

	bool bIsJumped;			// 점프모션중인지 체크용
	bool bAttackToggle;		// 공격 체크용 토글
	bool bHeavyToggle;		// 강공격 체크용 토글
	bool bNextAtkCheckOn;	// 다음 공격 입력 체크용
	bool bHeavyAtk;			// 약공격인지 강공격인지 판별(true = 강, false = 약)
	bool bAtkTrace;			// 공격 판정 체크

	// 공격 모션 도중 움직이지 못하는 상태 체크
	bool bNoInputInAtk;	// 공격 판정 모션과 공격 애니메이션 종료 사이에 움직일 수 있도록 해주기 위해

	bool bSprintToggle;		// 달리기 체크용 토글
	bool bItemDelay;		// 아이템 사용 딜레이 체크용
	bool bAtkRotate;		// 공격 중 회전 체크용
	FVector vAtkDir = FVector::ZeroVector;	// 공격 중 회전 방향

	// 구르기 관련
	bool bDodging;
	bool bDodgeMove;
	FVector vDodgeVector;
	FRotator rDodgeRotation;

	// 방패 방어 상태
	bool bToggleGuard;

	// 공격 관련
	int32 CurrentCombo;

	bool bShowMenu;

	FTimerHandle JumpAtkTimer;		// 점프공격 타이머
	FTimerHandle BlockReactTimer;	// 방어 표현 타이머
	FTimerHandle LockOnTimer;		// 락온 타이머
	FTimerHandle LockOnFailedTimer;	// 락온 실패 타이머
	FTimerHandle HitStiffTimer;		// 공격 적중시 모션 경직 타이머

	float fGuardPhysicsWeight;
	float fAttackPlayRate;

public:
	void SetbAtkTrace(const bool& _AtkTrace) { bAtkTrace = _AtkTrace;}
	void SetbAtkToggle(const bool& _AtkToggle) { bAttackToggle = _AtkToggle; }
	void SetbNextAtkCheck(const bool& _NextAtkCheck) { bNextAtkCheckOn = _NextAtkCheck; }
	// 조작 불가 상태
	void SetbNoInputInAtk(const bool& _NoInputInAtk) { bNoInputInAtk = _NoInputInAtk; }
	void SetbAtkRotate(const bool& _AtkRotate) { bAtkRotate = _AtkRotate; }
	// 이동 가능 상태
	void SetbIsJumped(const bool& _IsJumped) { bIsJumped = _IsJumped; }
	// 아이템 사용 딜레이
	bool GetbItemDelay() const { return bItemDelay; }
	void SetbItemDelay(const bool& _ItemDelay) { bItemDelay = _ItemDelay; }

	// 가드상태(블렌드 중이 아니라 완전 가드모션중일 때만 true)
	bool GetbToggleGuard() const { return bToggleGuard; }
	void SetbToggleGuard(const bool& _ToggleGuard);

	// 락온 중 플레이어가 적을 바라보고 있도록 설정
	//void SetOrientRotation(const bool& _Val);

	// 회피 애니메이션 종료 설정
	void SetbDodging(const bool& _Dodging) { bDodging = _Dodging; }

	const UCameraComponent* GetCamera() { return m_Cam; }

	// 락온 토글 상태 확인
	bool GetbToggleLockOn() const;

	FVector GetvAtkDir() const { return vAtkDir; }
	void SetvAtkDirZero()  { vAtkDir = FVector::ZeroVector; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType, EPlayerMontage _AtkMontage);
	void AttackHitCheck();				// 어택 트레이스용
	void NextAttackCheck();				// 다음 공격 발동 체크
	void DodgeTimeCheck(bool _Dodge);	// 회피 무적시간 체크
	void AttackMove();				// 공격 모션 중 이동
	bool ConsumeStaminaForMontage(EPlayerMontage _Montage); // 애니메이션별 스태미나 소비
	void JumpAttack();

	UFUNCTION()
	void TargetLockOn();

public:	
	void GainMonsterSoul(int32 _GainedSoul);
	// 공격 트레이스에 피격된 대상 목록 초기화
	void EmptyHitActorArr() { HitActorArr.Empty(); }
	void CloseMenuUI();	
	bool BlockEnemyAttack(float _Damage, FVector _MonDir); // 적 공격 방어
	void UseItem(EITEM_ID _ID, EEQUIP_SLOT _Slot);

	UFUNCTION()
	void ItemDelaytime(float _DelayPercent);

	void BreakLockOn();
	void ShotProjectile();
	void StopSprint();
	void ResetCamera(FRotator _Rotate);

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
