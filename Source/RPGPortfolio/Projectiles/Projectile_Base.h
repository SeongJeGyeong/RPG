// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	USphereComponent* m_Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UProjectileMovementComponent* m_ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UStaticMeshComponent* m_Mesh;

public:	
	// Sets default values for this actor's properties
	AProjectile_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USphereComponent* GetHitSphere() const { return m_Sphere; }
};
