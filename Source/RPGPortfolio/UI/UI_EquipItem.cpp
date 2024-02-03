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
		switch ( eSlotType )
		{
		case EEQUIP_SLOT::WEAPON_1:
			ItemSlotText->SetText(FText::FromString(L"무기 1"));
			break;
		case EEQUIP_SLOT::WEAPON_2:
			ItemSlotText->SetText(FText::FromString(L"무기 2"));
			break;
		case EEQUIP_SLOT::WEAPON_3:
			ItemSlotText->SetText(FText::FromString(L"무기 3"));
			break;
		case EEQUIP_SLOT::SHIELD_1:
			ItemSlotText->SetText(FText::FromString(L"방패 1"));
			break;
		case EEQUIP_SLOT::SHIELD_2:
			ItemSlotText->SetText(FText::FromString(L"방패 2"));
			break;
		case EEQUIP_SLOT::SHIELD_3:
			ItemSlotText->SetText(FText::FromString(L"방패 3"));
			break;
		case EEQUIP_SLOT::ARROW:
			ItemSlotText->SetText(FText::FromString(L"화살"));
			break;
		case EEQUIP_SLOT::BOLT:
			ItemSlotText->SetText(FText::FromString(L"볼트"));
			break;
		case EEQUIP_SLOT::HELM:
			ItemSlotText->SetText(FText::FromString(L"투구"));
			break;
		case EEQUIP_SLOT::CHEST:
			ItemSlotText->SetText(FText::FromString(L"갑옷"));
			break;
		case EEQUIP_SLOT::GAUNTLET:
			ItemSlotText->SetText(FText::FromString(L"장갑"));
			break;
		case EEQUIP_SLOT::LEGGINGS:
			ItemSlotText->SetText(FText::FromString(L"각반"));
			break;
		case EEQUIP_SLOT::ACCESSORIE_1:
			ItemSlotText->SetText(FText::FromString(L"악세사리 1"));
			break;
		case EEQUIP_SLOT::ACCESSORIE_2:
			ItemSlotText->SetText(FText::FromString(L"악세사리 2"));
			break;
		case EEQUIP_SLOT::ACCESSORIE_3:
			ItemSlotText->SetText(FText::FromString(L"악세사리 3"));
			break;
		case EEQUIP_SLOT::ACCESSORIE_4:
			ItemSlotText->SetText(FText::FromString(L"악세사리 4"));
			break;
		case EEQUIP_SLOT::CONSUMABLE_1:
			ItemSlotText->SetText(FText::FromString(L"소비아이템 1"));
			break;
		case EEQUIP_SLOT::CONSUMABLE_2:
			ItemSlotText->SetText(FText::FromString(L"소비아이템 2"));
			break;
		case EEQUIP_SLOT::CONSUMABLE_3:
			ItemSlotText->SetText(FText::FromString(L"소비아이템 3"));
			break;
		case EEQUIP_SLOT::CONSUMABLE_4:
			ItemSlotText->SetText(FText::FromString(L"소비아이템 4"));
			break;
		case EEQUIP_SLOT::CONSUMABLE_5:
			ItemSlotText->SetText(FText::FromString(L"소비아이템 5"));
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
