// Fill out your copyright notice in the Description page of Project Settings.


#include "Proj_Player_Cutter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../System/DamageType_Base.h"

// Sets default values
AProj_Player_Cutter::AProj_Player_Cutter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	m_Hitbox->SetCollisionProfileName("PlayerRangeAtk");
	SetRootComponent(m_Hitbox);
	m_Hitbox->SetEnableGravity(false);

	m_BaseNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	m_BaseNiagara->SetupAttachment(m_Hitbox);

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	m_ProjectileMovement->ProjectileGravityScale = 0.f;
}

// Called when the game starts or when spawned
void AProj_Player_Cutter::BeginPlay()
{
	Super::BeginPlay();
	
	m_Hitbox->OnComponentHit.AddDynamic(this, &AProj_Player_Cutter::OnHitProj);
}

// Called every frame
void AProj_Player_Cutter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	fDestroyTime += DeltaTime;

	if (fDestroyTime > 1.f)
	{
		Destroy();
	}
}

void AProj_Player_Cutter::LaunchMotion(FVector _LaunchVec)
{
	m_ProjectileMovement->Velocity = _LaunchVec;
}

void AProj_Player_Cutter::OnHitProj(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GetProjHitNiagara(), Hit.Location);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetProjHitSound(), GetActorLocation());
	if (GetOwner() == nullptr)
	{
		return;
	}

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(eAtkType);
	UGameplayStatics::ApplyPointDamage(Hit.GetActor(), fAtkDamage, Hit.Normal, Hit, GetOwner()->GetInstigatorController(), this, DamageTypeBase);
	Destroy();
}