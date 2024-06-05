// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Base.generated.h"


USTRUCT(Atomic)
struct FEquipmentStat
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Helm_PhyDef = 0.f;
	UPROPERTY()
	float Helm_MagDef = 0.f;
	UPROPERTY()
	float Chest_PhyDef = 0.f;
	UPROPERTY()
	float Chest_MagDef = 0.f;
	UPROPERTY()
	float Gaunt_PhyDef = 0.f;
	UPROPERTY()
	float Gaunt_MagDef = 0.f;
	UPROPERTY()
	float Leg_PhyDef = 0.f;
	UPROPERTY()
	float Leg_MagDef = 0.f;
	UPROPERTY()
	float Wea_PhyAtk = 0.f;
	UPROPERTY()
	float Wea_MagAtk = 0.f;
};
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

	FEquipmentStat EquipmentStat;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	int32 GetPlayerLevel() const { return m_PlayerStat.Level; }
	int32 GetPlayerSoul() const { return m_PlayerBasePower.AmountOfSoul; }

	FCharacterStatSheet GetPlayerStatus() const { return m_PlayerStat; }
	FCharacterBasePower GetPlayerBasePower() const { return m_PlayerBasePower; }

	void SetPlayerStat(FCharacterStatSheet _PlayerStat);
	void SetPlayerBasePower();
	void InitPlayerData(FCharacterBasePower _PlayerBasePower);
	void SetEquipFigure(FGameItemInfo* _ItemInfo, bool bEquiped);
	void SetPlayerCurrentHP(float _CurHP);
	void SetPlayerCurrentMP(float _CurMP);
	void SetPlayerCurrentStamina(float _CurStamina);
	void PlayerGainSoul(int32 _Soul);

	const FEquipmentStat& GetEquipmentStat() { return EquipmentStat; }

	bool GetbSTRecovery() const { return bSTRecovery; }
	void SetbSTRecovery(const bool& _STRecovery) { bSTRecovery = _STRecovery; }
	bool GetbSTRecovSlowly() const { return bSTRecovSlowly; }
	void SetbSTRecovSlowly(const bool& _STRecovSlowly) { bSTRecovSlowly = _STRecovSlowly; }

};
