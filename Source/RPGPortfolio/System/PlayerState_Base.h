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
	FCharacterStatSheet PlayerStat;
	UPROPERTY()
	FCharacterBasePower PlayerBasePower;

public:
	int32 GetPlayerLevel() const;
	int32 GetPlayerSoul() const;

	void InitPlayerData(FCharacterStatSheet _PlayerStat);
	void SetPlayerStat(FCharacterStatSheet _PlayerStat);

};
