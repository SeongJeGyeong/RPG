// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "../System/DataAsset/DA_MonsterInfo.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API AMonster_Base : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FMonsterInfo	m_Info;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	EMONSTER_TYPE	m_Type;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle	m_MonsterInfoTableRow;

	UPROPERTY()
	UDataTable* m_ItemTable;

	UPROPERTY()
	TArray<struct FMonsterItemDropTable> m_DropItemArr;

	UPROPERTY()
	class ULockOnTargetComponent* m_TargetComp;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* m_WidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UDA_MonsterInfo>	m_DataAssetInfo;

	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	EITEM_ID m_DropItemID;
	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	int32 m_DropItemStack = 1;

	UPROPERTY()
	UAnimInstance* m_AnimInst;

	UPROPERTY()
	class UWidgetComponent* m_LockOnMarker;
	UPROPERTY()
	class UUI_Monster* m_MonsterWidget;
	
	EMONSTER_STATE	m_State;

	float fDeadEffectRatio;
	float fWidgetVisTime;
	bool bLockedOn;
	bool bAtkTrace;
	bool bIsDead;

	FTimerHandle DeadTimer;
	FTimerHandle HitEndTimer;

protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_BehaviroTree;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBlackboardData* m_Blackboard;

	UPROPERTY()
	class AAIC_Monster_Base* m_AIController;

public:
	UBehaviorTree* GetBehaviorTree() const { return m_BehaviroTree; }
	UBlackboardData* GetBlackboard() const { return m_Blackboard; }
	const FMonsterInfo& GetMonsterInfo() { return m_Info; }
	EMONSTER_STATE GetState() { return m_State; }

	bool GetbLockedOn() const { return bLockedOn; }
	void SetbLockedOn(bool _LockedOn);

	bool GetbAtkTrace() const { return bAtkTrace; }
	void SetbAtkTrace(bool _AtkTrace) { bAtkTrace = _AtkTrace; }

	bool GetbIsDead() const { return bIsDead; }

	void ChangeState(EMONSTER_STATE _State) { m_State = _State; }
	void MeleeAttackHitCheck();

public:
	// Sets default values for this character's properties
	AMonster_Base();

protected:

	virtual void OnConstruction(const FTransform& _Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void MonsterAttackNormal();
	
	void MonsterDead(AActor* DamageCauser);
	void MonsterDestroy();

	UFUNCTION()
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnBlockMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
