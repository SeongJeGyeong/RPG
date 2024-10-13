// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI_EquipItemList.h"
#include "UI_ItemTooltip.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"
#include "../Manager/Equip_Mgr.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../System/DataAsset/DA_ItemCategoryIcon.h"
#include "PaperSprite.h"

void UUI_EquipItem::NativeConstruct()
{
	if (!IsValid(m_EquipItemBtn) || !IsValid(m_EquipItemImg) || !IsValid(m_EquipDishImg) || !IsValid(m_BackSlotImg))
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 아이템 UI 로드 실패"));
	}

	UPaperSprite* pIcon = UInventory_Mgr::GetInst(GetWorld())->GetEquipSlotIcon(eSlotType);
	m_BackSlotImg->SetBrushResourceObject(pIcon);
	if (!m_EquipItemBtn->GetIsEnabled())
	{
		m_EquipItemBtn->SetBackgroundColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 0.5f));
	}
	else
	{
		UItem_InvenData* pItemInfo = UEquip_Mgr::GetInst(GetWorld())->GetEquipItemFromSlot(eSlotType);
		if (pItemInfo != nullptr)
		{
			FString ItemImgPath = pItemInfo->GetItemImgPath();
			UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
			m_EquipItemImg->SetBrushFromTexture(pTex2D);
			m_EquipItemImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			m_EquipDishImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			m_EquipItemImg->SetVisibility(ESlateVisibility::Hidden);
			m_EquipDishImg->SetVisibility(ESlateVisibility::Hidden);
		}

		m_EquipItemBtn->OnClicked.AddDynamic(this, &UUI_EquipItem::ItemBtnClicked);
		m_EquipItemBtn->OnHovered.AddDynamic(this, &UUI_EquipItem::ItemBtnHovered);
		m_EquipItemBtn->OnUnhovered.AddDynamic(this, &UUI_EquipItem::ItemBtnUnHovered);
	}

	Super::NativeConstruct();
}

void UUI_EquipItem::SetEquipItem(UItem_InvenData* _ItemData)
{
	if (!IsValid(_ItemData))
	{
		m_EquipItemImg->SetVisibility(ESlateVisibility::Hidden);
		m_EquipDishImg->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		FString ItemImgPath = _ItemData->GetItemImgPath();
		UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
		m_EquipItemImg->SetBrushFromTexture(pTex2D);
		m_EquipItemImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		m_EquipDishImg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUI_EquipItem::ItemBtnClicked()
{
	ItemList->SetSlotCategory(eSlotType);
	EITEM_TYPE Type = EITEM_TYPE::ALL;
	switch ( eSlotType )
	{
	case EEQUIP_SLOT::WEAPON_1:
		ItemList->SetCategoryText(FText::FromString(L"무기 1"));
		Type = EITEM_TYPE::WEAPON;
		break;
	case EEQUIP_SLOT::WEAPON_2:
		ItemList->SetCategoryText(FText::FromString(L"무기 2"));
		Type = EITEM_TYPE::WEAPON;
		break;
	case EEQUIP_SLOT::WEAPON_3:
		ItemList->SetCategoryText(FText::FromString(L"무기 3"));
		Type = EITEM_TYPE::WEAPON;
		break;
	case EEQUIP_SLOT::SHIELD_1:
		ItemList->SetCategoryText(FText::FromString(L"방패 1"));
		Type = EITEM_TYPE::SHIELD;
		break;
	case EEQUIP_SLOT::SHIELD_2:
		ItemList->SetCategoryText(FText::FromString(L"방패 2"));
		Type = EITEM_TYPE::SHIELD;
		break;
	case EEQUIP_SLOT::SHIELD_3:
		ItemList->SetCategoryText(FText::FromString(L"방패 3"));
		Type = EITEM_TYPE::SHIELD;
		break;
	case EEQUIP_SLOT::ARROW:
		ItemList->SetCategoryText(FText::FromString(L"화살"));
		Type = EITEM_TYPE::ARROWS;
		break;
	case EEQUIP_SLOT::BOLT:
		ItemList->SetCategoryText(FText::FromString(L"볼트"));
		Type = EITEM_TYPE::ARROWS;
		break;
	case EEQUIP_SLOT::HELM:
		ItemList->SetCategoryText(FText::FromString(L"투구"));
		Type = EITEM_TYPE::ARM_HELM;
		break;
	case EEQUIP_SLOT::CHEST:
		ItemList->SetCategoryText(FText::FromString(L"갑옷"));
		Type = EITEM_TYPE::ARM_CHEST;
		break;
	case EEQUIP_SLOT::GAUNTLET:
		ItemList->SetCategoryText(FText::FromString(L"장갑"));
		Type = EITEM_TYPE::ARM_GAUNTLET;
		break;
	case EEQUIP_SLOT::LEGGINGS:
		ItemList->SetCategoryText(FText::FromString(L"각반"));
		Type = EITEM_TYPE::ARM_LEGGINGS;
		break;
	case EEQUIP_SLOT::ACCESSORIE_1:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 1"));
		Type = EITEM_TYPE::ACCESSORIE;
		break;
	case EEQUIP_SLOT::ACCESSORIE_2:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 2"));
		Type = EITEM_TYPE::ACCESSORIE;
		break;
	case EEQUIP_SLOT::ACCESSORIE_3:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 3"));
		Type = EITEM_TYPE::ACCESSORIE;
		break;
	case EEQUIP_SLOT::ACCESSORIE_4:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 4"));
		Type = EITEM_TYPE::ACCESSORIE;
		break;
	case EEQUIP_SLOT::CONSUMABLE_1:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 1"));
		Type = EITEM_TYPE::CONSUMABLE;
		break;
	case EEQUIP_SLOT::CONSUMABLE_2:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 2"));
		Type = EITEM_TYPE::CONSUMABLE;
		break;
	case EEQUIP_SLOT::CONSUMABLE_3:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 3"));
		Type = EITEM_TYPE::CONSUMABLE;
		break;
	case EEQUIP_SLOT::CONSUMABLE_4:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 4"));
		Type = EITEM_TYPE::CONSUMABLE;
		break;
	case EEQUIP_SLOT::CONSUMABLE_5:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 5"));
		Type = EITEM_TYPE::CONSUMABLE;
		break;
	default:
		break;
	}
	UInventory_Mgr::GetInst(GetWorld())->RenewEquipItemListUI(Type);
	ItemList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
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

	UItem_InvenData* pItemInfo = UEquip_Mgr::GetInst(GetWorld())->GetEquipItemFromSlot(eSlotType);

	if (IsValid(ItemNameText) && IsValid(pItemInfo))
	{
		FString sItemName = pItemInfo->GetItemName();
		ItemNameText->SetText(FText::FromString(sItemName));
		m_Tooltip->SetTooltipUI(pItemInfo);
		m_Tooltip->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	
	PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));
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
	if (m_Tooltip->GetVisibility() == ESlateVisibility::HitTestInvisible )
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
	}
}
