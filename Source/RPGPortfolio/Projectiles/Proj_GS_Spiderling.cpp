// Fill out your copyright notice in the Description page of Project Settings.


#include "Proj_GS_Spiderling.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../System/DamageType_Base.h"

AProj_GS_Spiderling::AProj_GS_Spiderling()
{
	PrimaryActorTick.bCanEverTick = true;

	m_Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	m_Hitbox->SetCollisionProfileName("MonsterRangeAtk");
	SetRootComponent(m_Hitbox);
	m_Hitbox->SetEnableGravity(true);

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_Particle->SetupAttachment(m_Hitbox);

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	m_ProjectileMovement->ProjectileGravityScale = 1.f;
}

void AProj_GS_Spiderling::BeginPlay()
{
	Super::BeginPlay();

	m_Hitbox->OnComponentHit.AddDynamic(this, &AProj_GS_Spiderling::OnHitProj);
}

void AProj_GS_Spiderling::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProj_GS_Spiderling::LaunchMotion(FVector _TargetVec)
{
	FVector LaunchVel;
	// 타겟을 향해 포물선을 그리며 날아가도록 Velocity를 계산
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, LaunchVel, GetActorLocation(), _TargetVec);

	m_ProjectileMovement->Velocity = LaunchVel;
}

void AProj_GS_Spiderling::OnHitProj(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("HitProj"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetProjHitParticle(), Hit.Location);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetProjHitSound(), GetActorLocation());
	if ( GetOwner() == nullptr )
	{
		return;
	}

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(eAtkType);
	UGameplayStatics::ApplyPointDamage(Hit.GetActor(), fAtkDamage, Hit.Normal, Hit, GetOwner()->GetInstigatorController(), this, DamageTypeBase);
	Destroy();
}
