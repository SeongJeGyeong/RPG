// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
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

	// UPROPERTY 리플렉션을 붙여주어야 가비지 컬렉션에서 관리되며, 해당 멤버가 포함된 클래스와 동일한 수명주기를 갖게 된다.
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UButton* m_EquipItemBtn;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UImage* m_EquipItemImg;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UImage* m_EquipDishImg;

	UPROPERTY()
	class UItem_InvenData* m_ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UTextBlock* ItemSlotText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UTextBlock* ItemNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UUI_EquipItemList*	ItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UUI_ItemTooltip* m_Tooltip;	// 장비창 메인화면 블루프린트에서 참조
	UPROPERTY()
	class UDA_MenuSound* m_Sound;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;
	void SlotVisibilityChanged(ESlateVisibility _Visibility);

	void SetItemSlotUI(class UTextBlock* _SlotText) { ItemSlotText = _SlotText; }
	void SetItemNameUI(class UTextBlock* _NameText) { ItemNameText = _NameText; }
	void SetItemListUI(class UUI_EquipItemList* _ItemList) { ItemList = _ItemList; }

	void SetSlotType(EEQUIP_SLOT _Type) { eSlotType = _Type; }
	EEQUIP_SLOT GetSlotType() { return eSlotType; }
	class UButton* GetItemBtn() { return m_EquipItemBtn; }

	void SetEquipItem(UItem_InvenData* _ItemData);
	void SetEquipItemStack(const uint16& _Stack);

	UFUNCTION()
	void ItemBtnClicked();
	UFUNCTION()
	void ItemBtnHovered();
	UFUNCTION()
	void ItemBtnUnHovered();
};
