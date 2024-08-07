// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipItemList.generated.h"

class UTextBlock;
struct FInvenItemRow;
/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_EquipItemList : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG", meta = ( AllowPrivateAccess = true ))
	class UUI_ItemTooltip* m_Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG", meta = ( AllowPrivateAccess = true ))
	class UUI_PlayerStat* m_Stat;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_SlotName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_List_ItemName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTileView*	m_EquipItemTileView;

	EEQUIP_SLOT		eEquipSlot;
	class UDA_MenuSound* m_Sound;

public:
	void SetSlotCategory(EEQUIP_SLOT _EquipSlot) { eEquipSlot = _EquipSlot; }
	void SetCategoryText(FText _Text);
	virtual void NativeConstruct() override;

	void AddEquipItemList(UObject* _ItemData);
	void ClearTileView();
	void OnTileHovered(UObject* _ItemData, bool _Hovered);
};
