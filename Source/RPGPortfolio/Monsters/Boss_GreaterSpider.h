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
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UParticleSystemComponent* m_PSC;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDA_GreaterSpider>	m_DataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UMotionWarpingComponent* m_MWComponent;

	UPROPERTY()
	class UAnim_GreaterSpider* m_AnimInst;

	UPROPERTY()
	TArray<AActor*> HitActorArr;

	UPROPERTY()
	TSubclassOf<class AProj_GS_Spiderling> m_GSProj;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	float RangedAtkRange; // 원거리 공격 거리

	EGreaterSpider_STATE m_State;
	bool bAtkTrace;
	bool bRushMove;
	bool bPhase2;

public:
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

private:
	void MeleeAttackHitCheck();
	void SweepAtkTrace(FName _Start, FName _End, float _Radius);
	void RushAttack(bool _Rush);
	void AttackSphereHitCheck(FVector _Location,float _Radius);
	void RangedAttack();
	void SwingAttack(EGreaterSpider_STATE _state);
	void StompAttack();

	void MonsterDead(AController* EventInstigator);

	void PlayGSSound(EGreaterSpider_STATE _state);
	void SpawnGSEffect(EGreaterSpider_STATE _state);

public:
	void ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType, EGreaterSpider_STATE _AtkState);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void PlayGSMontage(EGreaterSpider_STATE _State);
	void DeadCollisionSetting();

};
