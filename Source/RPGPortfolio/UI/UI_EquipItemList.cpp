// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItemList.h"
#include "Components/TextBlock.h"
#include "Components/TileView.h"

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

void UUI_EquipItemList::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
}
