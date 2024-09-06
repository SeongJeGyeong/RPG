// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Base.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AProjectile_Base::AProjectile_Base()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile_Base::PlayHitEffect(bool _HittedActor, FVector _HitLocation, FRotator _EffrecRotation)
{
	UFXSystemAsset* _EffectSystem = _HittedActor ? m_HitEffect_Actor : m_HitEffect_Wall;

	if (!IsValid(_EffectSystem) || !IsValid(m_HitSound))
	{
		return;
	}

	if ( (_EffectSystem)->IsA(UNiagaraSystem::StaticClass()) )
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cast<UNiagaraSystem>(_EffectSystem), _HitLocation, _EffrecRotation);
	}
	else if ( (_EffectSystem)->IsA(UParticleSystem::StaticClass()) )
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cast<UParticleSystem>(_EffectSystem), _HitLocation);
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_HitSound, GetActorLocation());
}