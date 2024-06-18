// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Header/Struct.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_Base.generated.h"

class UInventory_Mgr;
class UEquip_Mgr;

UCLASS()
class RPGPORTFOLIO_API UGameInstance_Base : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	UInventory_Mgr*		m_InvenMgr;
	UPROPERTY()
	UEquip_Mgr*			m_EquipMgr;
	UPROPERTY()
	TSubclassOf<class UUserWidget>	m_LoadingScreenClass;

	UPROPERTY()
	FCharacterStatSheet PlayerStat;
	UPROPERTY()
	FCharacterBasePower PlayerBasePower;

	UPROPERTY()
	FEquipmentStat EquipmentStat;

	UPROPERTY()
	uint32 PlayTime;

public:
	UGameInstance_Base();
	~UGameInstance_Base();

	virtual void Init() override;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	FCharacterStatSheet GetPlayerStatus() const { return PlayerStat; }
	void SetPlayerStatus(const FCharacterStatSheet& _PlayerStat) { PlayerStat = _PlayerStat; }

	FCharacterBasePower GetPlayerBasePower() const { return PlayerBasePower; }
	void SetPlayerBasePower(const FCharacterBasePower& _PlayerBasePower) { PlayerBasePower = _PlayerBasePower; }

	FEquipmentStat GetEquipmentStatInfo() const { return EquipmentStat; }
	void SetEquipmentStatInfo(const FEquipmentStat& _EquipmentStat) { EquipmentStat = _EquipmentStat; }

	uint32 GetPlayTime() const { return PlayTime; }
	void SetPlayTime(const uint32& _PlayTime) { PlayTime = _PlayTime; }


	// Inventory_Mgr에서 GameInstance의 private 멤버를 사용할 수 있음
	friend class UInventory_Mgr;
	friend class UEquip_Mgr;
};
