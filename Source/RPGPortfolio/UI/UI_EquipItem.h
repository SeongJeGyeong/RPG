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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	EEQUIP_SLOT	eSlotType;

	class UButton*	m_ItemBtn;
	class UImage*	m_ItemImg;
	class UImage*	m_DishImg;

	class UItem_InvenData* m_ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UTextBlock* ItemSlotText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UTextBlock* ItemNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UUI_EquipItemList*	ItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UUI_ItemTooltip* m_Tooltip;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void SetItemSlotUI(class UTextBlock* _SlotText) { ItemSlotText = _SlotText; }
	void SetItemNameUI(class UTextBlock* _NameText) { ItemNameText = _NameText; }
	void SetItemListUI(class UUI_EquipItemList* _ItemList) { ItemList = _ItemList; }
	void SetSlotType(EEQUIP_SLOT _Type) { eSlotType = _Type; }
	EEQUIP_SLOT GetSlotType() { return eSlotType; }
	class UButton* GetItemBtn() { return m_ItemBtn; }

	UFUNCTION()
	void ItemBtnClicked();
	UFUNCTION()
	void ItemBtnHovered();
	UFUNCTION()
	void ItemBtnUnHovered();
};
