// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "../System/DataAsset/DA_GreaterSpider.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss_Base.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDamagedBossHPDelegate, float, float); // 보스 체력 갱신 델리게이트

class UBehaviorTree;
class UBlackboardData;

UCLASS()
class RPGPORTFOLIO_API ABoss_Base : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	FDataTableRowHandle	m_MonsterInfoTableRow;

protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* m_BehaviroTree;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardData* m_Blackboard;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( DisplayName = "공격 가능 범위" ))
	float m_AtkRange;		// 공격 가능범위

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( DisplayName = "원거리 공격 가능 범위" ))
	float m_RangedAtkRange; // 원거리 공격 거리

	FMonsterInfo m_Info;

	float StaggerGauge = 0.f;
	float fPhysicsWeight = 1.f;
	FTimerHandle HitReactTimer;
	FTimerDelegate HitReactDelegate;
	
	FTimerHandle DeadTimer;
	float fDeadEffectRatio;

private:
	bool bBossDead;
	int32 iTurnDir;	// 포즈 블렌딩용 변수

public:
	FOnDamagedBossHPDelegate OnDamagedBossHP;

public:
	class UBehaviorTree* GetBehaviorTree() { return m_BehaviroTree; }
	class UBlackboardData* GetBlackboard() { return m_Blackboard; }

	void SetBossLockedOn(const bool& _LockedOn);

	bool GetbBossDead() const {return bBossDead;}

	int32 GetiTurnDir() const { return iTurnDir; }
	void SetiTurnDir(const int32& _TurnDir) { iTurnDir = _TurnDir; }

public:
	// Sets default values for this character's properties
	ABoss_Base();

protected:
	virtual void OnConstruction(const FTransform& _Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MonsterDead();
	void DestroyBoss();
public:
	void StopBoneHitReaction(FName _BoneName);
};
