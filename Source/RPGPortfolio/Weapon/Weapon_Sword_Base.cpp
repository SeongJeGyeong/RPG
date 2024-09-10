// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Sword_Base.h"

// Sets default values
AWeapon_Sword_Base::AWeapon_Sword_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	m_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	RootComponent = m_WeaponMesh;

	m_WeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AWeapon_Sword_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_Sword_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

