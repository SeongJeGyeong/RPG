// Fill out your copyright notice in the Description page of Project Settings.

#include "../Header/Struct.h"
#include "UI_Player_QuickSlotItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"

void UUI_Player_QuickSlotItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_Dish = Cast<UImage>(GetWidgetFromName("dish"));
	m_ItemImg = Cast<UImage>(GetWidgetFromName("ItemImg"));
	m_ItemName = Cast<UTextBlock>(GetWidgetFromName("ItemName"));
	m_Amount = Cast<UTextBlock>(GetWidgetFromName("Amount"));

	if (!IsValid(m_Dish) || !IsValid(m_ItemImg) || !IsValid(m_ItemName) || !IsValid(m_Amount))
	{
		UE_LOG(LogTemp, Error, TEXT("퀵슬롯 아이템 UI 캐스팅 실패"));
	}
	else
	{
		m_Dish->SetVisibility(ESlateVisibility::Hidden);
		m_ItemImg->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_Player_QuickSlotItem::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Player_QuickSlotItem::RenewQuickSlotItem(FInvenItemRow* _InvenItem)
{
	if (_InvenItem == nullptr)
	{
		m_Dish->SetVisibility(ESlateVisibility::Hidden);
		m_ItemImg->SetVisibility(ESlateVisibility::Hidden);
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_Amount->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FString ItemImgPath = _InvenItem->ItemInfo->IconImgPath;
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);
	m_ItemName->SetText(FText::FromString(_InvenItem->ItemInfo->ItemName));
	m_Amount->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenItem->Stack)));
	m_Dish->SetVisibility(ESlateVisibility::Visible);
	m_ItemImg->SetVisibility(ESlateVisibility::Visible);
	m_ItemName->SetVisibility(ESlateVisibility::Visible);
	m_Amount->SetVisibility(ESlateVisibility::Visible);
}