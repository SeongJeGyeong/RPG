// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_StatMgr.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_StatMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UGISubsystem_StatMgr();

	FCharacterStatSheet PlayerStat;
	FCharacterBasePower PlayerBasePower;
	FEquipmentStat		EquipmentStat;

	UPROPERTY()
	class UUI_Player_Main*	PlayerMainUI;

	ERecoveryType RecoveryType = ERecoveryType::DEFAULT;
	FTimerHandle RecoveryTimer;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void SetUIInManager();

	int32 GetPlayerLevel() const { return PlayerStat.Level; }
	//int32 GetPlayerSoul() const { return PlayerBasePower.AmountOfSoul; }
	FString GetPlayerName() const { return PlayerStat.PlayerName; }

	FCharacterStatSheet GetPlayerStatus() const { return PlayerStat; }
	void SetPlayerStatus(const FCharacterStatSheet& _PlayerStat) { PlayerStat = _PlayerStat; }

	FCharacterBasePower GetPlayerBasePower() const { return PlayerBasePower; }
	void SetPlayerBasePower(const FCharacterBasePower& _PlayerBasePower) { PlayerBasePower = _PlayerBasePower; }

	void SetAtkAndDef();
	void SetEquipFigure(FGameItemInfo* _ItemInfo, bool bEquiped);
	void SetPlayerCurrentHP(float _CurHP);
	void SetPlayerCurrentMP(float _CurMP);
	void SetPlayerCurrentStamina(float _CurStamina);
	void PlayerGainSoul(int32 _Soul);

	const FEquipmentStat& GetEquipmentStatus() { return EquipmentStat; }

	UFUNCTION()
	void StaminaRecoveryStart();

	void SetbSTRecovSlowly(const bool& _bSlow) { RecoveryType = _bSlow ? ERecoveryType::SLOW : ERecoveryType::DEFAULT; }
};
