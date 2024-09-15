// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../System/Interface/PoolableObj.h"
#include "../System/DataAsset/DA_ProjectileAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_Base.generated.h"

class UFXSystemAsset;

// 추상클래스로 선언
UCLASS(Abstract)
class RPGPORTFOLIO_API AProjectile_Base : public AActor, public IPoolableObj
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	EATTACK_TYPE	m_AtkType;

	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	EProjectileType	m_ProjType;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = ( AllowPrivateAccess = "true" ))
	UFXSystemAsset* m_HitEffect_Actor;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = ( AllowPrivateAccess = "true" ))
	UFXSystemAsset* m_HitEffect_Wall;

	UPROPERTY(EditAnywhere, Category = "Sound", meta = ( AllowPrivateAccess = "true" ))
	USoundBase* m_HitSound;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UProjectileMovementComponent* m_ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	float fLifeTime = 0.f;
	FTimerHandle LifeTimeTimer;

public:	
	// Sets default values for this actor's properties
	AProjectile_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	EATTACK_TYPE GetAtkType() const { return m_AtkType; }
	void SetAtkType(EATTACK_TYPE _AtkType) { m_AtkType = _AtkType; }
	EProjectileType GetProjType() const { return m_ProjType; }
	void SetProjType(EProjectileType _ProjType) { m_ProjType = _ProjType; }

public:
	// PURE_VIRTUAL : 순수 가상함수 선언 매크로. 추상클래스로 선언된 UClass에서만 가능. 파라미터는 가상함수로 선언할 함수와 디폴트 리턴값. 
	virtual void LaunchMotion(FVector _LaunchVec) PURE_VIRTUAL(AProjectile_Base::LaunchMotion, );

	void PlayHitEffect(bool _HittedActor, FVector _HitLocation, FRotator _EffectRotation = FRotator::ZeroRotator);

	void SetActiveOnSpawn(const bool& _Active);
	void SetLifeTime(const float& _LifeTime) { fLifeTime = _LifeTime; }

	UFUNCTION()
	void ProjDeactive();

	void ProjReturnToPool();

public:
	virtual void OnSpawnObjFromPool(bool _Activate) override;
	virtual void OnReturnObjToPool() override;

};