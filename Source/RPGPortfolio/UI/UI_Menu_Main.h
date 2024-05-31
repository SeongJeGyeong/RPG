// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Menu_Main.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Menu_Main : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UButton*	m_Equip;
	class UButton*	m_Inventory;
	class UButton*	m_Status;
	class UButton*	m_Manual;
	class UButton*	m_Settings;
	class UTextBlock* m_MenuName;

	class UDA_MenuSound* m_Sound;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

public:
	UFUNCTION()
	void EquipBtnClicked();
	UFUNCTION()
	void EquipBtnHovered();
	UFUNCTION()
	void EquipBtnUnHovered();

	UFUNCTION()
	void InventoryBtnClicked();
	UFUNCTION()
	void InventoryBtnHovered();
	UFUNCTION()
	void InventoryBtnUnHovered();

	UFUNCTION()
	void StatusBtnClicked();
	UFUNCTION()
	void StatusBtnHovered();
	UFUNCTION()
	void StatusBtnUnHovered();

	UFUNCTION()
	void ManualBtnClicked();
	UFUNCTION()
	void ManualBtnHovered();
	UFUNCTION()
	void ManualBtnUnHovered();

	UFUNCTION()
	void SettingsBtnClicked();
	UFUNCTION()
	void SettingsBtnHovered();
	UFUNCTION()
	void SettingsBtnUnHovered();
};
