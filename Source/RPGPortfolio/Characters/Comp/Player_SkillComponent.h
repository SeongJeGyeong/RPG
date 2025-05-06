// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"
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
	UPROPERTY()
	class UDA_PlayerSkill* m_SkillDA;

	FSkillAsset* m_Skill;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FSkillAsset* GetEquippedSkill() const { return m_Skill; }
	ESkillName GetSkillName() const { return m_SkillName; }

	void ShotProjectile();
};
