// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipItemList.generated.h"


struct FInvenItemRow;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_EquipItemList : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG", meta = ( AllowPrivateAccess = true ))
	class UUI_ItemTooltip* m_Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG", meta = ( AllowPrivateAccess = true ))
	class UUI_PlayerStat* m_Stat;

	class UTextBlock*	m_SlotName;
	class UTextBlock*	m_ListItemName;
	class UTileView*	m_TileView;
	EEQUIP_SLOT		eEquipSlot;

	class UDA_MenuSound* m_Sound;

public:
	void SetSlotCategory(EEQUIP_SLOT _EquipSlot) { eEquipSlot = _EquipSlot; }
	void SetCategoryText(FText _Text);
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void AddEquipItemList(UObject* _ItemData);
	void ClearTileView();
	//void RenewItemListUI(EITEM_TYPE _Type);
	void OnTileHovered(UObject* _ItemData, bool _Hovered);
};
