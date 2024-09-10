// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon_Sword_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API AWeapon_Sword_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Sword_Base();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = ( AllowPrivateAccess = "true" ))
	USkeletalMeshComponent* m_WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	float					m_Extent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
