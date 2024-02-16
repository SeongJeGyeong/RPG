// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItemList.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"
#include "UI_ItemTooltip.h"
#include "UI_InvenItem.h"

void UUI_EquipItemList::NativeConstruct()
{
	Super::NativeConstruct();

	m_SlotName = Cast<UTextBlock>(GetWidgetFromName(TEXT("SlotName")));
	m_ListItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("List_ItemName")));
	m_TileView = Cast<UTileView>(GetWidgetFromName(TEXT("EquipItemTileView")));

	if (!IsValid(m_SlotName) || !IsValid(m_ListItemName) || !IsValid(m_TileView))
	{
		UE_LOG(LogTemp, Error, TEXT("장착아이템 리스트 캐스팅 실패"));
	}
	else
	{
		m_TileView->OnItemIsHoveredChanged().AddUObject(this, &UUI_EquipItemList::OnTileHovered);
	}

}

void UUI_EquipItemList::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_EquipItemList::SetCategoryText(FText _Text)
{
	m_SlotName->SetText(_Text);
}

void UUI_EquipItemList::AddEquipItemList(UObject* _ItemData)
{
	m_TileView->AddItem(_ItemData);
}

void UUI_EquipItemList::ClearTileView()
{
	if (IsValid(m_TileView))
	{
		m_TileView->ClearListItems();
	}
}

void UUI_EquipItemList::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
	UUI_InvenItem* ItemUI = Cast<UUI_InvenItem>(m_TileView->GetEntryWidgetFromItem(_ItemData));
	if (_Hovered)
	{
		// 장비할 아이템목록에서는 메뉴앵커 표시 안되도록
		ItemUI->SetAnchorActive(false);
		ItemUI->SetSelectedSlot(eEquipSlot);

		UItem_InvenData* pData = Cast<UItem_InvenData>(_ItemData);

		m_ListItemName->SetText(FText::FromString(pData->GetItemName()));
		m_ListItemName->SetVisibility(ESlateVisibility::Visible);
		m_Tooltip->SetTooltipUI(pData);
		m_Tooltip->SetVisibility(ESlateVisibility::Visible);
		
	}
	else
	{
		m_ListItemName->SetVisibility(ESlateVisibility::Hidden);
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
		ItemUI->SetSelectedSlot(EEQUIP_SLOT::EMPTY);
	}
}
