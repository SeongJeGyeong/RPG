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
class UUI_Menu_Main;

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

	UPROPERTY()
	class UAudioComponent* m_BGMComp;

public:
	UUI_Base* GetMainHUD() { return m_MainHUD; }
	UUI_Inventory* GetInventoryUI() { return m_InventoryUI; }
	UUI_StatusMain* GetStatusUI() { return m_StatusUI; }
	UUI_EquipMain* GetEquipUI() { return m_EquipUI; }
	UUI_Manual* GetManualUI() { return m_ManualUI; }
	UUI_Settings* GetSettingsUI() { return m_SettingsUI; }
	bool IsSubMenuUIOpened();
	void CloseSubMenu();

	void EquipUI_SetVisibility(bool _IsOpen);
	void InventoryUI_SetVisibility(bool _IsOpen);
	void StatusUI_SetVisibility(bool _IsOpen);
	void ManualUI_SetVisibility(bool _IsOpen);
	void SettingsUI_SetVisibility(bool _IsOpen);

	void BindMenuUI(UUI_Menu_Main* _MenuUI);

	void PlayBGM(bool _Play);

public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

};
