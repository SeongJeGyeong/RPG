// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API APlayerState_Base : public APlayerState
{
	GENERATED_BODY()
	
public:
	APlayerState_Base();

private:
	UPROPERTY()
	FCharacterStatSheet m_PlayerStat;
	UPROPERTY()
	FCharacterBasePower m_PlayerBasePower;

public:
	int32 GetPlayerLevel() const { return m_PlayerStat.Level; }
	int32 GetPlayerSoul() const { return m_PlayerBasePower.AmountOfSoul; }

	FCharacterStatSheet GetPlayerStatus() const { return m_PlayerStat; }
	FCharacterBasePower GetPlayerBasePower() const { return m_PlayerBasePower; }

	void SetPlayerStat(FCharacterStatSheet _PlayerStat);
	void SetPlayerBasePower(EEQUIP_SLOT _Slot);
	void InitPlayerData(FCharacterBasePower _PlayerBasePower);
};
