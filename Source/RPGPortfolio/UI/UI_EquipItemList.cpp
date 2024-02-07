// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItemList.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"
#include "UI_ItemTooltip.h"

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

void UUI_EquipItemList::RenewItemListUI(EITEM_TYPE _Type)
{
	if ( !IsValid(m_TileView) )
	{
		UE_LOG(LogTemp, Error, TEXT("장착할 아이템 타일뷰 불러오지 못함"));
		return;
	}
	else
	{
		m_TileView->ClearListItems();

	}

	TMap<EITEM_ID, FInvenItemRow> InvenStorageMap[(int32)EITEM_ID::END];

	for ( int32 i = 0; i < (int32)EITEM_ID::END; ++i )
	{
		UInventory_Mgr::GetInst(GetWorld())->GetInvenStorage(InvenStorageMap[i], i);

		for (auto Iter = InvenStorageMap[i].CreateConstIterator(); Iter; ++Iter)
		{
			UItem_InvenData* pItemData = NewObject<UItem_InvenData>();
			if (Iter.Value().ItemInfo->Type == _Type)
			{
				pItemData->SetItemImgPath(Iter.Value().ItemInfo->IconImgPath);
				pItemData->SetItemName(Iter.Value().ItemInfo->ItemName);
				pItemData->SetItemDesc(Iter.Value().ItemInfo->Description);
				pItemData->SetItemQnt(Iter.Value().Stack);
				pItemData->SetAtkVal(Iter.Value().ItemInfo->ATK);
				pItemData->SetDefVal(Iter.Value().ItemInfo->DEF);
				pItemData->SetRestoreHP(Iter.Value().ItemInfo->Restore_HP);
				pItemData->SetRestoreMP(Iter.Value().ItemInfo->Restore_MP);
				pItemData->SetRequireStr(Iter.Value().ItemInfo->Require_Str);
				pItemData->SetRequireDex(Iter.Value().ItemInfo->Require_Dex);
				pItemData->SetRequireInt(Iter.Value().ItemInfo->Require_Int);
				pItemData->SetMaximumStack(Iter.Value().ItemInfo->Maximum_Stack);
				pItemData->SetItemType(Iter.Value().ItemInfo->Type);
			}
			else
			{
				continue;
			}
			m_TileView->AddItem(pItemData);
		}
	}
}

void UUI_EquipItemList::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
	if (_Hovered)
	{
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
	}
}
