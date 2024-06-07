// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItemList.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"
#include "UI_ItemTooltip.h"
#include "UI_InvenItem.h"
#include "UI_PlayerStat.h"
#include "../System/DataAsset/DA_MenuSound.h"

void UUI_EquipItemList::NativeConstruct()
{

	if (!IsValid(m_SlotName) || !IsValid(m_List_ItemName) || !IsValid(m_EquipItemTileView))
	{
		UE_LOG(LogTemp, Error, TEXT("장착아이템 리스트 캐스팅 실패"));
	}
	else
	{
		m_EquipItemTileView->OnItemIsHoveredChanged().AddUObject(this, &UUI_EquipItemList::OnTileHovered);
	}

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if ( !IsValid(m_Sound) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비아이템리스트 사운드 로드 실패"));
	}

	Super::NativeConstruct();
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
	m_EquipItemTileView->AddItem(_ItemData);
}

void UUI_EquipItemList::ClearTileView()
{
	if (IsValid(m_EquipItemTileView))
	{
		m_EquipItemTileView->ClearListItems();
	}
}

void UUI_EquipItemList::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
	UUI_InvenItem* ItemUI = Cast<UUI_InvenItem>(m_EquipItemTileView->GetEntryWidgetFromItem(_ItemData));
	if (_Hovered)
	{
		// 장비할 아이템목록에서는 메뉴앵커 표시 안되도록
		ItemUI->SetAnchorActive(false);
		ItemUI->SetSelectedSlot(eEquipSlot);

		UItem_InvenData* pData = Cast<UItem_InvenData>(_ItemData);

		if (eEquipSlot != EEQUIP_SLOT::ARROW && eEquipSlot != EEQUIP_SLOT::BOLT && eEquipSlot != EEQUIP_SLOT::CONSUMABLE_1 &&
			eEquipSlot != EEQUIP_SLOT::CONSUMABLE_2 && eEquipSlot != EEQUIP_SLOT::CONSUMABLE_3 &&
			eEquipSlot != EEQUIP_SLOT::CONSUMABLE_4 && eEquipSlot != EEQUIP_SLOT::CONSUMABLE_5
			)
		{
			ItemUI->SetStatUI(m_Stat);
			// 이미 장착된 아이템에 호버될 경우 장착 해제시 변경될 능력치 수치를 표시하도록 한다.
			if (pData->GetEquiped() == eEquipSlot)
			{
				m_Stat->AlterRenewBasePower(pData, true);
			}
			else
			{
				m_Stat->AlterRenewBasePower(pData, false);
			}
			
			m_Stat->SetVisibilityAlterBasePower(true);
		}

		m_List_ItemName->SetText(FText::FromString(pData->GetItemName()));
		m_List_ItemName->SetVisibility(ESlateVisibility::Visible);
		m_Tooltip->SetTooltipUI(pData);
		m_Tooltip->SetVisibility(ESlateVisibility::Visible);

		PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
	}
	else
	{
		m_List_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
		ItemUI->SetSelectedSlot(EEQUIP_SLOT::EMPTY);

		m_Stat->SetVisibilityAlterBasePower(false);
	}
}
