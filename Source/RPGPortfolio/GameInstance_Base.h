// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"
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

	uint32 PlayTime;

	UPROPERTY()
	USoundClass* m_MasterVolume;

	float fTempVolume;
	FIntPoint TempResolution;

	FStreamableManager AssetStreamManager;
	FTimerHandle StreamTimer;
public:
	UGameInstance_Base();
	~UGameInstance_Base();

	virtual void Init() override;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	uint32 GetPlayTime() const { return PlayTime; }
	void SetPlayTime(const uint32& _PlayTime) { PlayTime = _PlayTime; }

	UFUNCTION()
	void ASyncLoadDataAsset(FSoftObjectPath _AssetPath);

	float GetMasterVolume() const;
	void SetMasterVolume(const float& _Volume);
	void LoadMasterVolume();
	void ApplyMasterVolume();
	void SetTempVolume(const float& _Volume) { fTempVolume = _Volume; }
	FIntPoint GetTempResolution() const { return TempResolution; }
	void SetTempResolution(const FIntPoint& _Res) { TempResolution = _Res; }
	void ExecuteResoltionCommand();

	void RestartPlayer();

private:
	void AssetLoaded(FString _AssetName);

	// Inventory_Mgr에서 GameInstance의 private 멤버를 사용할 수 있음
	friend class UInventory_Mgr;
	friend class UEquip_Mgr;
};
