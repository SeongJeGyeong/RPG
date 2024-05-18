// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Base.h"

// Sets default values
AProjectile_Base::AProjectile_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	// 액터 컴포넌트인 투사체 무브 컴포넌트 추가
	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	SetRootComponent(m_Sphere);
	m_Mesh->SetupAttachment(m_Sphere);

}

// Called when the game starts or when spawned
void AProjectile_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

