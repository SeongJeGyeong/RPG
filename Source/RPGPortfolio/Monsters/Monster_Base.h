// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../Header/Struct.h"
#include "Components/TimelineComponent.h"
#include "../System/DataAsset/PDA_MonsterAssets.h"
#include "../System/DataAsset/DA_MonsterInfo.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API AMonster_Base : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	EMONSTER_TYPE	m_Type;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle	m_MonsterInfoTableRow;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	class UWidgetComponent* m_WidgetComponent;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	float m_DetectRange;	// 플레이어 탐지 범위

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	float m_AtkRange;		// 공격 가능범위

	UPROPERTY()
	UDataTable* m_ItemTable;

	UPROPERTY()
	TArray<struct FMonsterItemDropTable> m_DropItemArr;

	UPROPERTY()
	class UAnimInstance_Monster_Base* m_AnimInst;

	UPROPERTY()
	class UUI_Monster* m_MonsterWidget;
	
	UPROPERTY()
	UTimelineComponent* m_HitTimeline;

	UPROPERTY()
	class UCurveVector* m_HitCurve;

	UPROPERTY()
	FMonsterInfo	m_Info;
	UPROPERTY()
	FMonsterAnimAsset m_AnimAsset;

	struct FDroppedItemData	m_DropItemInfo;

	FVector PrevTraceLoc;
	FVector RelLoc;
	FOnTimelineVector HitTimelineCallback;
	FOnTimelineEvent TimelineFinishCallback;

	float fDeadEffectRatio;
	bool bMonLockedOn;
	bool bAtkTrace;
	bool bMonDead;

	FTimerHandle DeadTimer;
	FTimerHandle MonHitStiffTimer;
	FTimerHandle HitEndTimer;
	FTimerHandle WidgetDisplayTimer;

protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_BehaviroTree;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBlackboardData* m_Blackboard;

public:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "variable")
	bool bDebug;

public:
	UBehaviorTree* GetBehaviorTree() const { return m_BehaviroTree; }
	UBlackboardData* GetBlackboard() const { return m_Blackboard; }
	const FMonsterInfo& GetMonsterInfo() { return m_Info; }

	void SetMonLockedOn(bool _LockedOn);
	void SetbAtkTrace(bool _AtkTrace) { bAtkTrace = _AtkTrace; }
	bool GetbMonDead() const { return bMonDead; }

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
	void PlayAtkBlockedAnim();

private:
	void MeleeAttackHitCheck();

protected:
	void MonsterDead(AController* EventInstigator);

	UFUNCTION()
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnBlockMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void TimelineStep(FVector _Value);

	UFUNCTION()
	void TimelineFinished();
};
