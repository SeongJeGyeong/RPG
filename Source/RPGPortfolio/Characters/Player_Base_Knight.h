// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "State/StateMachine.h"
#include "GenericTeamAgentInterface.h"
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
class UPlayer_SkillComponent;
class ULockOnTargetComponent;
class UMotionWarpingComponent;
class UUI_Base;
class IPlayerInteraction;

UCLASS()
class RPGPORTFOLIO_API APlayer_Base_Knight : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer_Base_Knight();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UPlayer_CameraArm* m_SArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UPlayer_SkillComponent* m_SkillComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UMotionWarpingComponent* m_MWComponent;

	UPROPERTY(EditAnywhere, Category = "Input", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UInputMappingContext> m_IMC;

	UPROPERTY(EditAnywhere, Category = "Input", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UDA_InputAction>	m_IA_Setting;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = ( AllowPrivateAccess = "true" ))
	UDA_PlayerMontage* m_PlayerMontage;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = ( AllowPrivateAccess = "true" ))
	UDA_PlayerSound* m_PlayerSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UCapsuleComponent* HitCollision;

	UPROPERTY()
	UAnimInstance_Knight* m_AnimInst;

	UPROPERTY()
	TArray<TScriptInterface<IPlayerInteraction>> OverlapInteractionArr;	// 오버랩된 상호작용 오브젝트 목록

	UPROPERTY()
	UUI_Base* m_MainUI;

	UPROPERTY()
	TArray<AActor*> HitActorArr;

	// UObject 클래스와는 호환이 안되므로 일반 c++ 클래스를 추상클래스로 만들어 사용
	TUniquePtr<StateMachine> CurrentState;

private:
	bool bIsAttacking;
	bool bNextAtkStart;
	bool bHeavyHold;		// 강공격 체크용
	bool bNextAtkCheckOn;	// 다음 공격 입력 체크용
	bool bAtkTrace;			// 공격 판정 체크

	bool bSprint;		// 달리기 체크용 토글
	bool bItemDelay;		// 아이템 사용 딜레이 체크용

	float fForwardSpeed;
	float fRightSpeed;

	// 구르기 관련
	//bool bDodging;
	//bool bDodgeMove;

	// 방패 방어 상태(가드를 완전히 올린 상태에서만 true)
	bool bInputGuard;
	bool bHoldGuard;
	float fGuardWeight;

	// 공격 관련
	uint8 CurrentCombo;

	FTimerHandle JumpAtkTimer;		// 점프공격 타이머
	FTimerHandle BlockReactTimer;	// 방어 표현 타이머
	FTimerHandle LockOnFailedTimer;	// 락온 실패 타이머
	FTimerHandle HitStiffTimer;		// 공격 적중시 모션 경직 타이머
	FTimerHandle ItemDelayTimer;

	float fDelayRate;
	FVector PrevTraceLoc;
public:
	UDA_PlayerMontage* GetMontageDA() const { return m_PlayerMontage; }
	UDA_PlayerSound* GetSoundDA() const { return m_PlayerSound; }
	UMotionWarpingComponent* GetMotionWarpingComp() const { return m_MWComponent; }
	bool GetbIsAttacking() const { return bIsAttacking; }
	void SetbIsAttacking(const bool& _IsAttacking) { bIsAttacking = _IsAttacking; }

	bool GetbAtkTrace() const { return bAtkTrace; }
	void SetbAtkTrace(const bool& _AtkTrace) { bAtkTrace = _AtkTrace; }
	void ResetPrevTraceLoc() { PrevTraceLoc = FVector::ZeroVector; }

	bool GetbNextAtkStart() const { return bNextAtkStart; }
	void SetbNextAtkStart(const bool& _NextAtkStart) { bNextAtkStart = _NextAtkStart; }

	uint8 GetCurrentCombo() const { return CurrentCombo; }
	void SetCurrentCombo(const uint8& _Combo) { CurrentCombo = _Combo; }

	void SetbNextAtkCheck(const bool& _NextAtkCheck) { bNextAtkCheckOn = _NextAtkCheck; }
	// 아이템 사용 딜레이
	bool GetbItemDelay() const { return bItemDelay; }
	void SetbItemDelay(const bool& _ItemDelay) { bItemDelay = _ItemDelay; }

	bool GetbInputGuard() const { return bInputGuard; }
	// 가드상태(블렌드 중이 아니라 완전 가드모션중일 때만 true)
	bool GetbHoldGuard() const { return bHoldGuard; }
	void SetbHoldGuard(const bool& _HoldGuard);
	float GetfGuardWeight() const { return fGuardWeight; }
	void SetfGuardWeight(const float& _GuardWeight) { fGuardWeight = _GuardWeight; }

	bool GetbSprint() const { return bSprint; }
	void SetbSprint(const bool& _bSprint) { bSprint = _bSprint; }

	float GetfForwardSpeed() const { return fForwardSpeed; }
	float GetfRightSpeed() const { return fRightSpeed; }

	UCameraComponent* GetCamera() const { return m_Cam; }
	//UPlayer_CameraArm* GetCameraArm() const { return m_SArm; }
	// 락온 토글 상태 확인
	bool GetbToggleLockOn() const;
	ULockOnTargetComponent* GetLockOnTarget() const;

	UAnimInstance_Knight* GetAnimInst() const { return m_AnimInst; }

	// 공격 트레이스에 피격된 대상 목록 초기화
	void EmptyHitActorArr() { HitActorArr.Empty(); }

protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetState(EPlayerStateType _StateType);

private:
	void MoveAction(const FInputActionInstance& _Instance);
	void RotateAction(const FInputActionInstance& _Instance);
	void JumpAction(const FInputActionInstance& _Instance);
	void SprintAction(const FInputActionInstance& _Instance);
	void GuardAction(const FInputActionInstance& _Instance);
	void AttackAction(const FInputActionInstance& _Instance);
	void HeavyAttack(const FInputActionInstance& _Instance);
	void DodgeAction(const FInputActionInstance& _Instance);
	void ParryAction(const FInputActionInstance& _Instance);
	void LockOnToggleAction(const FInputActionInstance& _Instance);
	void SwitchLockOnTarget(const FInputActionInstance& _Instance);
	void OpenMenu(const FInputActionInstance& _Instance);
	void ActionCommand(const FInputActionInstance& _Instance);
	void QuickSlotChange(const FInputActionInstance& _Instance);
	void UseLowerQuickSlot(const FInputActionInstance& _Instance);
	void UseSkill_1(const FInputActionInstance& _Instance);

	void AttackMove();				// 공격 모션 중 이동
	void JumpAttack();
	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType, EPlayerMontage _AtkMontage);

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void AttackHitCheck();				// 어택 트레이스용
	void AttackStart();
	void HitBoxActivate(bool _Activate);

	bool ConsumeStaminaForMontage(EPlayerMontage _Montage); // 애니메이션별 스태미나 소비
	bool ConsumeStamina(float _Consumption);
	bool ConsumeMP(float _Consumption);
	bool BlockEnemyAttack(float _Damage, FVector _MonDir); // 적 공격 방어
	void BreakLockOn();
	void ResetCamera(FRotator _Rotate);
	void ShotProjectile();
	void UseItem(EITEM_ID _ID, EEQUIP_SLOT _Slot);
	UFUNCTION()
	void ItemDelaytime(float _DelayPercent);
	void GainMonsterSoul(int32 _GainedSoul);
	void CloseMenuUI();

	void ResetVarsOnHitState();
	void ClearMontageRelatedTimer(); // 몽타주 관련 타이머 clear
	void MotionWarping_Attack(UAnimSequenceBase* _Anim, float _EndTime);
	void MotionWarping_Dodge(UAnimSequenceBase* _Anim, float _EndTime);

private:
	void DodgeTimeCheck(bool _Dodge);	// 회피 무적시간 체크

	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void HitBoxBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	void ActionTriggerBeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult);

	UFUNCTION()
	void ActionTriggerEndOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index);

public:
	virtual FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(0); };	// 플레이어 팀 설정(0)

	void DrawDebugAttackTrace(const UWorld* World, const FVector& Start, const FVector& End, FQuat& Rot, const FCollisionShape& CollisionShape, bool bHit, const TArray<FHitResult>& OutHits, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
};
