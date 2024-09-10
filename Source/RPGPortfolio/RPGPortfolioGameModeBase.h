// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGPortfolioGameModeBase.generated.h"

class UUI_Base;
class UUI_Inventory;
class UUI_StatusMain;
class UUI_EquipMain;
class UUI_Manual;
class UUI_Settings;
class UUI_FadeScreen;


/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGPortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPGPortfolioGameModeBase();
	~ARPGPortfolioGameModeBase();

private:
	UPROPERTY()
	TArray<TSubclassOf<UUserWidget>> m_WidgetClassArr;

	UPROPERTY()
	UUI_Base* m_MainHUD;

	UPROPERTY()
	UUI_Inventory* m_InventoryUI;

	UPROPERTY()
	UUI_StatusMain* m_StatusUI;

	UPROPERTY()
	UUI_EquipMain* m_EquipUI;

	UPROPERTY()
	UUI_Manual* m_ManualUI;

	UPROPERTY()
	UUI_Settings* m_SettingsUI;

	//UPROPERTY()
	//UUI_FadeScreen* m_FadeScreenUI;

	UPROPERTY()
	class UAudioComponent* m_BGMComp;

	UPROPERTY()
	class USoundBase* m_BGM;

	FVector PlayerSpawnLoc;

public:
	UUI_Base* GetMainHUD() { return m_MainHUD; }
	UUI_Inventory* GetInventoryUI() { return m_InventoryUI; }
	UUI_StatusMain* GetStatusUI() { return m_StatusUI; }
	UUI_EquipMain* GetEquipUI() { return m_EquipUI; }
	UUI_Manual* GetManualUI() { return m_ManualUI; }
	UUI_Settings* GetSettingsUI() { return m_SettingsUI; }
	//UUI_FadeScreen* GetFadeUI() { return m_FadeScreenUI; }
	bool IsSubMenuUIOpened();
	void CloseSubMenu();

	void PlayBGM(bool _Play);

	void SetPlayerRespawnLoc(FVector _Loc);

public:
	virtual void BeginPlay() override;

};
