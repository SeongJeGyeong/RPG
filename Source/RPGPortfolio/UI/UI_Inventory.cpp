// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "Components/TileView.h"
#include "Components/TextBlock.h"
#include "../Item/Item_InvenData.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "../System/PlayerState_Base.h"
#include "Components/MenuAnchor.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	m_TileView = Cast<UTileView>(GetWidgetFromName(L"ItemTileView"));
	m_Tooltip = Cast<UUI_ItemTooltip>(GetWidgetFromName(L"ItemTooltipUI"));
	m_ItemName = Cast<UTextBlock>(GetWidgetFromName(L"ItemName"));
	m_Status = Cast<UUI_PlayerStat>(GetWidgetFromName(L"PlayerStatUI"));

	if (!IsValid(m_TileView))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 타일 뷰 찾지 못함"));
	}
	else
	{
		m_TileView->OnItemIsHoveredChanged().AddUObject(this, &UUI_Inventory::OnTileHovered);
	}

	if (!IsValid(m_Tooltip))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 툴팁 위젯 찾지 못함"));
	}
	else
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_ItemName))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 이름 위젯 찾지 못함"));
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_Status))
	{
		UE_LOG(LogTemp, Error, TEXT("캐릭터스탯 위젯 찾지 못함"));
	}

}

void UUI_Inventory::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Inventory::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
	if (_Hovered)
	{
		UItem_InvenData* pData = Cast<UItem_InvenData>(_ItemData);

		m_ItemName->SetText(FText::FromString(pData->GetItemName()));
		m_ItemName->SetVisibility(ESlateVisibility::Visible);

		m_Tooltip->SetTooltipUI(pData);
		m_Tooltip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUI_Inventory::AddItem(UObject* _ItemData)
{
	m_TileView->AddItem(_ItemData);
}

void UUI_Inventory::Clear()
{
	if (IsValid(m_TileView))
	{
		m_TileView->ClearListItems();
	}
}

bool UUI_Inventory::IsInventoryOpened()
{
	if (this->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UUI_Inventory::SetStatUI(APlayerState* _PlayerState)
{
	m_Status->SetPlayerStatUI(Cast<APlayerState_Base>(_PlayerState));
}
