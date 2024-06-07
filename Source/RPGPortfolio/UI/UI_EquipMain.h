// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
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
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock*		m_Sel_ItemName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock*		m_Sel_ItemSlot;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_ItemTooltip*	m_ItemTooltipUI;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_PlayerStat*	m_Equip_StatUI;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_EquipItemList* m_Equip_ItemListUI;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void ListVisibilityChanged(ESlateVisibility _Visibility);

public:
	UUI_EquipItemList* GetItemList() { return m_Equip_ItemListUI; }
	bool GetItemListVisibility();
	void CloseItemList();
	void RenewEquipItem(EEQUIP_SLOT _Slot, class UItem_InvenData* _ItemData = nullptr);
	void RenewEquipItemStack(EEQUIP_SLOT _Slot, uint16 _Stack);
};
