// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../Item/Item_InvenData.h"
#include "UI_EquipItemList.h"
#include "../Header/Enum.h"

void UUI_EquipItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemBtn = Cast<UButton>(GetWidgetFromName(TEXT("ItemBtn")));
	m_ItemImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemImg")));
	m_DishImg = Cast<UImage>(GetWidgetFromName(TEXT("DishImg")));

	if (!IsValid(m_ItemBtn) || !IsValid(m_ItemImg) || !IsValid(m_DishImg) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 아이템 캐스팅 실패"));
	}
	else
	{
		m_ItemImg->SetVisibility(ESlateVisibility::Hidden);
		m_DishImg->SetVisibility(ESlateVisibility::Hidden);
		m_ItemBtn->OnClicked.AddDynamic(this, &UUI_EquipItem::ItemBtnClicked);
		m_ItemBtn->OnHovered.AddDynamic(this, &UUI_EquipItem::ItemBtnHovered);
		m_ItemBtn->OnUnhovered.AddDynamic(this, &UUI_EquipItem::ItemBtnUnHovered);
	}
}

void UUI_EquipItem::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_EquipItem::ItemBtnClicked()
{
	ItemList->SetVisibility(ESlateVisibility::Visible);
}

void UUI_EquipItem::ItemBtnHovered()
{
	if (IsValid(ItemSlotText))
	{
		switch ( eCaterogy )
		{
		case EITEM_TYPE::CONSUMABLE:
			ItemSlotText->SetText(FText::FromString(L"소비아이템"));
			break;
		case EITEM_TYPE::WEAPON:
			ItemSlotText->SetText(FText::FromString(L"무기"));
			break;
		case EITEM_TYPE::ARMOR:
			ItemSlotText->SetText(FText::FromString(L"방어구"));
			break;
		case EITEM_TYPE::ACCESSORIE:
			ItemSlotText->SetText(FText::FromString(L"악세사리"));
			break;
		case EITEM_TYPE::ARROWS:
			ItemSlotText->SetText(FText::FromString(L"화살"));
			break;
		default:
			break;
		}
	}

	if (IsValid(ItemNameText) && IsValid(m_ItemData))
	{
		ItemNameText->SetText(FText::FromString(m_ItemData->GetItemName()));
	}
}

void UUI_EquipItem::ItemBtnUnHovered()
{
	if (IsValid(ItemSlotText))
	{
		ItemSlotText->SetText(FText::GetEmpty());
	}
	if (IsValid(ItemNameText))
	{
		ItemNameText->SetText(FText::GetEmpty());
	}
}
