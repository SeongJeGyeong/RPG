// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "../System/Interface/ProjectileInterface.h"
#include "CoreMinimal.h"
#include "Projectile_Base.h"
#include "Proj_Player_Cutter.generated.h"

UCLASS()
class RPGPORTFOLIO_API AProj_Player_Cutter : public AProjectile_Base, public IProjectileInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UBoxComponent* m_Hitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UProjectileMovementComponent* m_ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UNiagaraComponent* m_BaseNiagara;

	EATTACK_TYPE	eAtkType;
	float			fAtkDamage;
	float			fDestroyTime = 0.f;
public:	
	// Sets default values for this actor's properties
	AProj_Player_Cutter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void LaunchMotion(FVector _LaunchVec) override;
	void SetProjDamage(EATTACK_TYPE _AtkType, float _AtkDamage) { eAtkType = _AtkType; fAtkDamage = _AtkDamage; };

	UFUNCTION()
	void OnHitProj(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
