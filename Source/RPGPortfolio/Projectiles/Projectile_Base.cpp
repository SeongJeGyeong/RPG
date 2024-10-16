// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Base.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "../System/Subsys_ObjectPool.h"

// Sets default values
AProjectile_Base::AProjectile_Base()
{
	PrimaryActorTick.bCanEverTick = false;

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	m_ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile_Base::LaunchMotion(FVector _LaunchVec)
{
}

void AProjectile_Base::PlayHitEffect(bool _HittedActor, FVector _HitLocation, FRotator _EffectRotation)
{
	UFXSystemAsset* _EffectSystem = _HittedActor ? m_HitEffect_Actor : m_HitEffect_Wall;

	if (!IsValid(_EffectSystem) || !IsValid(m_HitSound))
	{
		return;
	}

	if ( _EffectSystem->IsA(UNiagaraSystem::StaticClass()) )
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Cast<UNiagaraSystem>(_EffectSystem), _HitLocation, _EffectRotation, FVector(1.f), true, true, ENCPoolMethod::AutoRelease);
	}
	else if ( _EffectSystem->IsA(UParticleSystem::StaticClass()) )
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cast<UParticleSystem>(_EffectSystem), _HitLocation, _EffectRotation, true, EPSCPoolMethod::AutoRelease);
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_HitSound, GetActorLocation());
}

void AProjectile_Base::SetActiveOnSpawn(const bool& _Active)
{
	SetActorHiddenInGame(!_Active);	// 액티브 되었을 경우 보여야 하므로 false가 되어야 함
	if (_Active)
	{
		if (fLifeTime > 0.f)
		{
			GetWorldTimerManager().SetTimer(LifeTimeTimer, this, &AProjectile_Base::ProjDeactive, fLifeTime, false);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(LifeTimeTimer);
		m_ProjectileMovement->StopMovementImmediately();
	}
}

void AProjectile_Base::ProjReturnToPool()
{
	if ( IsValid(GetWorld()) )
	{
		USubsys_ObjectPool* ObjectPool = GetWorld()->GetSubsystem<USubsys_ObjectPool>();
		ObjectPool->ReturnObjToPool(this);
	}
}

void AProjectile_Base::ProjDeactive()
{
	SetActorHiddenInGame(true);
	GetWorldTimerManager().ClearTimer(LifeTimeTimer);
	SetActorLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	m_ProjectileMovement->StopMovementImmediately();

	ProjReturnToPool();
}

void AProjectile_Base::OnSpawnObjFromPool(bool _Activate)
{
	SetActiveOnSpawn(_Activate);
}

void AProjectile_Base::OnReturnObjToPool()
{

}
