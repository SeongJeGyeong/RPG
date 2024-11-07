// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StreamableManager.h"
#include "Header/Struct.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API UGameInstance_Base : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSubclassOf<class UUserWidget>	m_LoadingScreenClass;

	uint32 PlayTime;

	UPROPERTY()
	USoundClass* m_MasterVolume;

	float fTempVolume;
	FIntPoint TempResolution;
	EWindowMode::Type TempWindowMode;

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
	EWindowMode::Type GetTempWindowMode() const { return TempWindowMode; }
	void SetTempWindowMode(const EWindowMode::Type& _WindowMode) { TempWindowMode = _WindowMode; }
	void ExecuteResoltionCommand();
	void RestartPlayer();

private:
	void AssetLoaded(FString _AssetName);
};
