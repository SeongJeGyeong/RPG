// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGPortfolioGameModeBase.generated.h"

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
	class UUI_Base* m_MainHUD;

	UPROPERTY()
	class UUI_Inventory* m_InventoryUI;

	UPROPERTY()
	class UUI_StatusMain* m_StatusUI;

	UPROPERTY()
	class UUI_EquipMain* m_EquipUI;

	UPROPERTY()
	class UUI_Manual* m_ManualUI;

	UPROPERTY()
	class UUI_Settings* m_SettingsUI;

	UPROPERTY()
	class UAudioComponent* m_BGMComp;

	UPROPERTY()
	class USoundBase* m_BGM;

public:
	class UUI_Base* GetMainHUD() { return m_MainHUD; }
	class UUI_Inventory* GetInventoryUI() { return m_InventoryUI; }
	class UUI_StatusMain* GetStatusUI() { return m_StatusUI; }
	class UUI_EquipMain* GetEquipUI() { return m_EquipUI; }
	class UUI_Manual* GetManualUI() { return m_ManualUI; }
	class UUI_Settings* GetSettingsUI() { return m_SettingsUI; }
	bool IsSubMenuUIOpened();
	void CloseSubMenu();

	void PlayBGM(bool _Play);

public:
	virtual void BeginPlay() override;

};
