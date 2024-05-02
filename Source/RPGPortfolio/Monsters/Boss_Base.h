// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "../System/DataAsset/DA_MonsterInfo.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Boss_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API ABoss_Base : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	EMONSTER_TYPE	m_Type;

	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	TSoftObjectPtr<UDA_MonsterInfo>	m_DataAssetInfo;

	UPROPERTY()
	UAnimInstance* m_AnimInst;

	UPROPERTY()
	class UWidgetComponent* m_LockOnMarker;

protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_BehaviroTree;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBlackboardData* m_Blackboard;

	UPROPERTY()
	class AAIC_Monster_Base* m_AIController;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float CurHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float PhysicAtk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float PhysicDef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MagicAtk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status", meta = (AllowPrivateAccess = "true"))
	float MagicDef;

	UPROPERTY()
	float StaggerGauge = 0.f;

public:
	bool bLockedOn;
	bool bAtkTrace;

public:
	UBehaviorTree* GetBehaviorTree() { return m_BehaviroTree; }
	UBlackboardData* GetBlackboard() { return m_Blackboard; }

	bool GetbLockedOn() { return bLockedOn; }
	void SetbLockedOn(bool _LockedOn);

	bool GetbAtkTrace() { return bAtkTrace; }
	void SetbAtkTrace(bool _AtkTrace) { bAtkTrace = _AtkTrace; }

public:
	// Sets default values for this character's properties
	ABoss_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void MeleeAttackHitCheck();

	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void MonsterDead(AActor* DamageCauser);
};
