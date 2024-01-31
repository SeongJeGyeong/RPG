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
	TSubclassOf<UUserWidget> m_MainHUDClass;
	class UUI_Base* m_MainHUD;

	TSubclassOf<UUserWidget> m_InventoryUIClass;
	class UUI_Inventory* m_InventoryUI;

	TSubclassOf<UUserWidget> m_StatusUIClass;
	class UUI_StatusMain* m_StatusUI;

	TSubclassOf<UUserWidget> m_EquipUIClass;
	class UUI_EquipMain* m_EquipUI;

public:
	class UUI_Base* GetMainHUD() { return m_MainHUD; }
	class UUI_Inventory* GetInventoryUI() { return m_InventoryUI; }
	class UUI_StatusMain* GetStatusUI() { return m_StatusUI; }
	class UUI_EquipMain* GetEquipUI() { return m_EquipUI; }

	bool IsSubMenuUIOpened();

public:
	virtual void BeginPlay() override;

};
