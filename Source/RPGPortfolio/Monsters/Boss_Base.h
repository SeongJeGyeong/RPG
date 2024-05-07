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

	UPROPERTY()
	class UWidgetComponent* m_LockOnMarker;

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
	class AAIC_Boss_Base* m_AIController;

	UPROPERTY()
	float StaggerGauge = 0.f;

private:
	bool bLockedOn;
	bool bIsDead;

public:
	UBehaviorTree* GetBehaviorTree() { return m_BehaviroTree; }
	UBlackboardData* GetBlackboard() { return m_Blackboard; }
	const FMonsterInfo& GetMonsterInfo() { return m_Info; }

	bool GetbLockedOn() {return bLockedOn;}
	void SetbLockedOn(bool _LockedOn);

	bool GetbIsDead() const {return bIsDead;}

public:
	// Sets default values for this character's properties
	ABoss_Base();

protected:
	virtual void OnConstruction(const FTransform& _Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void MonsterDead(AActor* DamageCauser);
};
