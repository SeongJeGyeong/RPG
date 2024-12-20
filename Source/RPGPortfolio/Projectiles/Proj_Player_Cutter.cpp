// Fill out your copyright notice in the Description page of Project Settings.


#include "Proj_Player_Cutter.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../System/DamageType_Base.h"

// Sets default values
AProj_Player_Cutter::AProj_Player_Cutter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	m_Hitbox->SetCollisionProfileName("PlayerRangeAtk");
	SetRootComponent(m_Hitbox);
	m_Hitbox->SetEnableGravity(false);

	m_BaseNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	m_BaseNiagara->SetupAttachment(m_Hitbox);
	fLifeTime = 1.f;
}

// Called when the game starts or when spawned
void AProj_Player_Cutter::BeginPlay()
{
	Super::BeginPlay();

	m_Hitbox->OnComponentHit.AddDynamic(this, &AProj_Player_Cutter::OnHitProj);
}

void AProj_Player_Cutter::LaunchMotion(FVector _LaunchVec)
{
	m_ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	m_ProjectileMovement->Velocity = _LaunchVec;
}

void AProj_Player_Cutter::OnHitProj(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	PlayHitEffect(true, Hit.ImpactPoint);

	if (GetOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("투사체 오너 없음"));
		return;
	}

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(eAtkType);
	UGameplayStatics::ApplyPointDamage(Hit.GetActor(), fAtkDamage, Hit.Normal, Hit, GetOwner()->GetInstigatorController(), this, DamageTypeBase);
	ProjDeactive();
}

void AProj_Player_Cutter::OnSpawnObjFromPool(bool _Activate)
{
	SetActiveOnSpawn(_Activate);
	m_BaseNiagara->ActivateSystem();
}

void AProj_Player_Cutter::OnReturnObjToPool()
{
	m_BaseNiagara->Deactivate();
}
