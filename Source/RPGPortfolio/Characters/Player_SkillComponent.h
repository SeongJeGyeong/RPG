// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_SkillComponent.generated.h"

//class AProj_Player_Cutter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayer_SkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayer_SkillComponent();

private:
	UPROPERTY()
	ESkillName m_SkillName;

	FSkillAsset* m_Skill;

	UPROPERTY()
	class UDA_PlayerSkill* m_SkillDA;

	/*UPROPERTY(EditAnywhere, Category = "Projectile", meta = ( AllowPrivateAccess = "true" ))
	TSubclassOf<AProj_Player_Cutter> ProjSubclass;*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void ShotSkillProj(FVector _SpawnLoc, FRotator _SpawnRot, FVector _ShotVec);

	FSkillAsset* GetEquippedSkill() const { return m_Skill; }
	ESkillName GetSkillName() const { return m_SkillName; }
};
