// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "Components/TileView.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	m_TileView = Cast<UTileView>(GetWidgetFromName(L"ItemTileView"));

	if (!IsValid(m_TileView))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 타일 뷰 찾지 못함"));
	}
	else
	{

	}
}

void UUI_Inventory::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
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
