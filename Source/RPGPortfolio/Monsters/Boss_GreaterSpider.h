// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss_Base.h"
#include "Boss_GreaterSpider.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ABoss_GreaterSpider : public ABoss_Base
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	UDA_GreaterSpider*	m_DataAsset;

	UPROPERTY()
	class UAnim_GreaterSpider* m_AnimInst;

	UPROPERTY()
	TArray<AActor*> HitActorArr;

	bool bAtkTrace;
	bool bRushTrace;
	bool bRushMove;
	EGreaterSpider_STATE m_State;
public:
	bool GetbAtkTrace() { return bAtkTrace; }
	void SetbAtkTrace(bool _AtkTrace) { bAtkTrace = _AtkTrace; }
	void EmptyHitArr() { HitActorArr.Empty(); }
public:
	// Sets default values for this character's properties
	ABoss_GreaterSpider();

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
	void MeleeAttackHitCheck();
	void LeftArmTrace();
	void RightArmTrace();
	void HeadTrace();
	void SweepArmTrace(FVector _Start, FVector _End);
	void RushAttackHitCheck();
	void PlayAttackMontage(EGreaterSpider_STATE _State);
	void MonsterDead(AController* EventInstigator);
	void DeadCollisionSetting();

	void RushAttack(bool _Rush);
};
