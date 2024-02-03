// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipMain.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_EquipMain : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock*		m_SelItemName;
	class UTextBlock*		m_SelItemSlot;
	class UUI_ItemTooltip*	m_Tooltip;
	class UUI_PlayerStat*	m_Stat;
	class UUI_EquipItemList* m_ItemList;

	class UUI_EquipItem* m_Weapon_1;
	class UUI_EquipItem* m_Weapon_2;
	class UUI_EquipItem* m_Weapon_3;
	class UUI_EquipItem* m_Shield_1;
	class UUI_EquipItem* m_Shield_2;
	class UUI_EquipItem* m_Shield_3;
	class UUI_EquipItem* m_Arrow;
	class UUI_EquipItem* m_Bolt;
	class UUI_EquipItem* m_Helm;
	class UUI_EquipItem* m_Chest;
	class UUI_EquipItem* m_Gauntlet;
	class UUI_EquipItem* m_Leggings;
	class UUI_EquipItem* m_Accessorie_1;
	class UUI_EquipItem* m_Accessorie_2;
	class UUI_EquipItem* m_Accessorie_3;
	class UUI_EquipItem* m_Accessorie_4;
	class UUI_EquipItem* m_Consumable_1;
	class UUI_EquipItem* m_Consumable_2;
	class UUI_EquipItem* m_Consumable_3;
	class UUI_EquipItem* m_Consumable_4;
	class UUI_EquipItem* m_Consumable_5;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void ListVisibilityChanged(ESlateVisibility _Visibility);
	void RenewItemListUI(EITEM_TYPE _Type);

	/*UFUNCTION()
	void ItemBtnClicked();*/

	void InitEquipSlot(class UUI_EquipItem* _SlotUI);

public:
	UUI_EquipItemList* GetItemList() { return m_ItemList; }
};
