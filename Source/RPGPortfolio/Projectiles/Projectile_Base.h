// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../System/DataAsset/DA_ProjectileAsset.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API AProjectile_Base : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	EATTACK_TYPE	m_AtkType;

	UPROPERTY(EditAnywhere, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	EProjectileType	m_ProjType;

	UPROPERTY(EditAnywhere, Category = "Assets", meta = ( AllowPrivateAccess = "true" ))
	UDA_ProjectileAsset* m_DA_Projectile;

	UPROPERTY()
	AActor* ShotInstigator;

public:	
	// Sets default values for this actor's properties
	AProjectile_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AActor* GetShotInstigator() const { return ShotInstigator; }
	void SetShotInstigator(AActor* _Instigator) { ShotInstigator = _Instigator; }

	EATTACK_TYPE GetAtkType() const { return m_AtkType; }
	void SetAtkType(EATTACK_TYPE _AtkType) { m_AtkType = _AtkType; }
	EProjectileType GetProjType() const { return m_ProjType; }
	void SetProjType(EProjectileType _ProjType) { m_ProjType = _ProjType; }
	UDA_ProjectileAsset* GetProjDA() const { return m_DA_Projectile; }
	UNiagaraSystem* GetProjBaseNiagara() const { return m_DA_Projectile->GetProjectileData(m_ProjType).ProjectileEffect; }
	UNiagaraSystem* GetProjHitNiagara() const { return m_DA_Projectile->GetProjectileData(m_ProjType).ProjHitEffect; }
	USoundBase* GetProjHitSound() const { return m_DA_Projectile->GetProjectileData(m_ProjType).ProjHitSound; }

};