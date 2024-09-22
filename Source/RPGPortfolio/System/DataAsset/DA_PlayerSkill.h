// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_PlayerSkill.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_PlayerSkill : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Skill", Meta = ( DisplayName = "SkillData" ))
	TMap<ESkillName, FSkillAsset> m_SkillMap;

public:
	FSkillAsset* GetPlayerSkill(ESkillName _SkillName) { return m_SkillMap.Find(_SkillName); }

};
