// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Menu_Main.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Menu_Main : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* m_Btn_Equip;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* m_Btn_Inventory;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* m_Btn_Status;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* m_Btn_Manual;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* m_Btn_Settings;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock* m_MenuName;

	UPROPERTY() 
	class UDA_MenuSound* m_Sound;

public:
	virtual void NativeConstruct() override;

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
