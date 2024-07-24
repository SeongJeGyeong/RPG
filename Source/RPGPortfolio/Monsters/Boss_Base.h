// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "../System/DataAsset/DA_GreaterSpider.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API ABoss_Base : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	FDataTableRowHandle	m_MonsterInfoTableRow;

protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info")
	EMONSTER_TYPE	m_Type;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_BehaviroTree;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBlackboardData* m_Blackboard;

	UPROPERTY()
	FMonsterInfo	m_Info;
	UPROPERTY()
	class UUI_Boss* m_BossWidget;

	UPROPERTY()
	float StaggerGauge = 0.f;
	UPROPERTY()
	float fPhysicsWeight = 1.f;
	FTimerHandle HitReactTimer;
	FTimerDelegate HitReactDelegate;
	
	FTimerHandle DeadTimer;
	float fDeadEffectRatio;

private:
	bool bBossDead;
	UPROPERTY()
	int32 iTurnDir;	// 포즈 블렌딩용 변수

public:
	class UBehaviorTree* GetBehaviorTree() { return m_BehaviroTree; }
	class UBlackboardData* GetBlackboard() { return m_Blackboard; }
	const FMonsterInfo& GetMonsterInfo() { return m_Info; }
	class UUI_Boss* GetBossWidget() { return m_BossWidget; }

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

public:
	void StopBoneHitReaction(FName _BoneName);
};
