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

	float Helm_PhyDef = 0.f;
	float Helm_MagDef = 0.f;
	float Chest_PhyDef = 0.f;
	float Chest_MagDef = 0.f;
	float Gaunt_PhyDef = 0.f;
	float Gaunt_MagDef = 0.f;
	float Leg_PhyDef = 0.f;
	float Leg_MagDef = 0.f;

	float Wea_PhyAtk = 0.f;
	float Wea_MagAtk = 0.f;

public:
	int32 GetPlayerLevel() const { return m_PlayerStat.Level; }
	int32 GetPlayerSoul() const { return m_PlayerBasePower.AmountOfSoul; }

	FCharacterStatSheet GetPlayerStatus() const { return m_PlayerStat; }
	FCharacterBasePower GetPlayerBasePower() const { return m_PlayerBasePower; }

	void SetPlayerStat(FCharacterStatSheet _PlayerStat);
	void SetPlayerBasePower();
	void InitPlayerData(FCharacterBasePower _PlayerBasePower);
	void SetEquipFigure(FGameItemInfo* _ItemInfo, bool bEquiped);

	const float& GetHelmPhyDef() { return Helm_PhyDef; }
	void SetHelmPhyDef(const float& _HelmPhyDef) { Helm_PhyDef = _HelmPhyDef; }
	const float& GetHelmMagDef() { return Helm_MagDef; }
	void SetHelmMagDef(const float& _HelmMagDef) { Helm_MagDef = _HelmMagDef; }

	const float& GetChestPhyDef() { return Chest_PhyDef; }
	void SetChestPhyDef(const float& _ChestPhyDef) { Chest_PhyDef = _ChestPhyDef; }
	const float& GetChestMagDef() { return Chest_MagDef; }
	void SetChestMagDef(const float& _ChestMagDef) { Chest_MagDef = _ChestMagDef; }

	const float& GetGauntPhyDef() { return Gaunt_PhyDef; }
	void SetGauntPhyDef(const float& _GauntPhyDef) { Gaunt_PhyDef = _GauntPhyDef; }
	const float& GetGauntMagDef() { return Gaunt_MagDef; }
	void SetGauntMagDef(const float& _GauntMagDef) { Gaunt_MagDef = _GauntMagDef; }

	const float& GetLegPhyDef() { return Leg_PhyDef; }
	void SetLegPhyDef(const float& _LegPhyDef) { Leg_PhyDef = _LegPhyDef; }
	const float& GetLegMagDef() { return Leg_MagDef; }
	void SetLegMagDef(const float& _LegMagDef) { Leg_MagDef = _LegMagDef; }

	const float& GetWeaPhyAtk() { return Wea_PhyAtk; }
	void SetWeaPhyAtk(const float& _WeaPhyAtk) { Wea_PhyAtk = _WeaPhyAtk; }
	const float& GetWeaMagAtk() { return Wea_MagAtk; }
	void SetWeaMagAtk(const float& _WeaMagAtk) { Wea_MagAtk = _WeaMagAtk; }
};
