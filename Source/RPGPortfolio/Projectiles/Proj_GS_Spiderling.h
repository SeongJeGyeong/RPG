// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "Projectile_Base.h"
#include "Proj_GS_Spiderling.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API AProj_GS_Spiderling : public AProjectile_Base
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class USphereComponent* m_Hitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UParticleSystemComponent* m_Particle;

	EATTACK_TYPE	eAtkType;
	float			fAtkDamage;

public:
	AProj_GS_Spiderling();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void LaunchMotion(FVector _TargetVec) override;
	void SetProjDamage(EATTACK_TYPE _AtkType, float _AtkDamage) { eAtkType = _AtkType; fAtkDamage = _AtkDamage; };

	UFUNCTION()
	void OnHitProj(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
