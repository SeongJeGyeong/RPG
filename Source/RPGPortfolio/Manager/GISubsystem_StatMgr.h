// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_StatMgr.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FRenewSoulDelegate, int32);	// 소지 소울 갱신 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FRenewHPDelegate, float);	// 현재 HP 갱신 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FRenewMPDelegate, float);	// 현재 MP 갱신 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FRenewSTDelegate, float);	// 현재 스태미나 갱신 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnRenewStatusDelegate);				// 스테이터스 갱신 델리게이트

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

	ERecoveryType RecoveryType = ERecoveryType::DEFAULT;
	FTimerHandle RecoveryTimer;

public:
	// UI_Base와 연결
	FRenewSoulDelegate OnRenewAmountSoul;
	FRenewHPDelegate OnRenewHP;
	FRenewMPDelegate OnRenewMP;
	FRenewSTDelegate OnRenewST;
	// UI_StatusMain
	FOnRenewStatusDelegate OnRenewStatus;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

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
