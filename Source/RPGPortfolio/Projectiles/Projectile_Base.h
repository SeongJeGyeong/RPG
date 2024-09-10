// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../System/DataAsset/DA_ProjectileAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_Base.generated.h"

class UFXSystemAsset;

UCLASS()
class RPGPORTFOLIO_API AProjectile_Base : public AActor
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
	void PlayHitEffect(bool _HittedActor, FVector _HitLocation, FRotator _EffrecRotation = FRotator::ZeroRotator);

};