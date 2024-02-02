// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_EquipItem : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UButton*	m_ItemBtn;
	class UImage*	m_ItemImg;
	class UImage*	m_DishImg;

	class UItem_InvenData* m_ItemData;
	EITEM_TYPE	eCaterogy;

	class UTextBlock* ItemSlotText;
	class UTextBlock* ItemNameText;
	class UUI_EquipItemList*	ItemList;
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void SetItemSlotUI(class UTextBlock* _SlotText) { ItemSlotText = _SlotText; }
	void SetItemNameUI(class UTextBlock* _NameText) { ItemNameText = _NameText; }
	void SetItemListUI(class UUI_EquipItemList* _ItemList) { ItemList = _ItemList; }
	void SetItemType(EITEM_TYPE _Type) { eCaterogy = _Type; }
	EITEM_TYPE GetItemType() { return eCaterogy; }
	class UButton* GetItemBtn() { return m_ItemBtn; }

	UFUNCTION()
	void ItemBtnClicked();
	UFUNCTION()
	void ItemBtnHovered();
	UFUNCTION()
	void ItemBtnUnHovered();
};
