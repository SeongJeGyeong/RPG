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
	UPROPERTY()
	class UUI_Player_Main* m_UI;

	float fSTRecoveryWait = 0.f;
	bool bSTRecovery = false;
	bool bSTRecovSlowly = false;

	UPROPERTY()
	FEquipmentStat m_EquipmentStat;

	UPROPERTY()
	uint32 PlayTime;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	int32 GetPlayerLevel() const { return m_PlayerStat.Level; }
	int32 GetPlayerSoul() const { return m_PlayerBasePower.AmountOfSoul; }
	FString GetPlayerName() const { return m_PlayerStat.PlayerName; }

	FCharacterStatSheet GetPlayerStatus() const { return m_PlayerStat; }
	FCharacterBasePower GetPlayerBasePower() const { return m_PlayerBasePower; }

	void SavePlayerStat(const FString& MapName);
	void SetAtkAndDef();
	void InitPlayerData(FCharacterBasePower _PlayerBasePower);
	void SetEquipFigure(FGameItemInfo* _ItemInfo, bool bEquiped);
	void SetPlayerCurrentHP(float _CurHP);
	void SetPlayerCurrentMP(float _CurMP);
	void SetPlayerCurrentStamina(float _CurStamina);
	void PlayerGainSoul(int32 _Soul);

	const FEquipmentStat& GetEquipmentStatus() { return m_EquipmentStat; }

	bool GetbSTRecovery() const { return bSTRecovery; }
	void SetbSTRecovery(const bool& _STRecovery) { bSTRecovery = _STRecovery; }
	bool GetbSTRecovSlowly() const { return bSTRecovSlowly; }
	void SetbSTRecovSlowly(const bool& _STRecovSlowly) { bSTRecovSlowly = _STRecovSlowly; }

	uint32 GetPlayTime() const { return PlayTime; }
	void SetPlayTime(const uint32& _PlayTime) { PlayTime = _PlayTime; }
};
