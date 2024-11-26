// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI_EquipItemList.h"
#include "UI_ItemTooltip.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../System/DataAsset/DA_ItemCategoryIcon.h"
#include "PaperSprite.h"
#include "../Manager/GISubsystem_InvenMgr.h"

void UUI_EquipItem::NativeConstruct()
{
	if (!IsValid(m_EquipItemBtn) || !IsValid(m_EquipItemImg) || !IsValid(m_EquipDishImg) || !IsValid(m_BackSlotImg))
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 아이템 UI 로드 실패"));
	}

	UGISubsystem_InvenMgr* pInvenMgr = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
	if ( !IsValid(pInvenMgr) )
	{
		UE_LOG(LogTemp, Error, TEXT("UUI_EquipItem : InvenMgr 가져오기 실패"));
	}

	UPaperSprite* pIcon = pInvenMgr->GetEquipSlotIcon(eSlotType);
	m_BackSlotImg->SetBrushResourceObject(pIcon);
	if (!m_EquipItemBtn->GetIsEnabled())
	{
		m_EquipItemBtn->SetBackgroundColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 0.5f));
	}
	else
	{
		UItem_InvenData* pItemInfo = pInvenMgr->GetEquipItemFromSlot(eSlotType);
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
	UGISubsystem_InvenMgr* pInvenMgr = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
	if ( IsValid(pInvenMgr) )
	{
		EITEM_TYPE Type = pInvenMgr->GetItemTypeFromSlot(eSlotType);
		FString SlotName = GetSlotText(eSlotType);
		ItemList->SetCategoryText(FText::FromString(SlotName));

		pInvenMgr->RenewEquipItemListUI(Type);
		ItemList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
	}
}

void UUI_EquipItem::ItemBtnHovered()
{
	if (IsValid(ItemSlotText))
	{
		FString SlotName = GetSlotText(eSlotType);
		ItemSlotText->SetText(FText::FromString(SlotName));
	}

	UGISubsystem_InvenMgr* pInvenMgr = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
	if ( IsValid(pInvenMgr) )
	{
		UItem_InvenData* pItemInfo = pInvenMgr->GetEquipItemFromSlot(eSlotType);
		if ( IsValid(ItemNameText) && IsValid(pItemInfo) )
		{
			FString sItemName = pItemInfo->GetItemName();
			ItemNameText->SetText(FText::FromString(sItemName));
			m_Tooltip->SetTooltipUI(pItemInfo);
			m_Tooltip->SetVisibility(ESlateVisibility::HitTestInvisible);
		}

		PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));
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
	if (m_Tooltip->GetVisibility() == ESlateVisibility::HitTestInvisible )
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
	}
}

FString UUI_EquipItem::GetSlotText(EEQUIP_SLOT _Slot)
{
	FString SlotName = L"전체";

	switch ( eSlotType )
	{
	case EEQUIP_SLOT::WEAPON_1:
		SlotName = L"무기 1";
		break;
	case EEQUIP_SLOT::WEAPON_2:
		SlotName = L"무기 2";
		break;
	case EEQUIP_SLOT::WEAPON_3:
		SlotName = L"무기 3";
		break;
	case EEQUIP_SLOT::SHIELD_1:
		SlotName = L"방패 1";
		break;
	case EEQUIP_SLOT::SHIELD_2:
		SlotName = L"방패 2";
		break;
	case EEQUIP_SLOT::SHIELD_3:
		SlotName = L"방패 3";
		break;
	case EEQUIP_SLOT::ARROW:
		SlotName = L"화살";
		break;
	case EEQUIP_SLOT::BOLT:
		SlotName = L"볼트";
		break;
	case EEQUIP_SLOT::HELM:
		SlotName = L"투구";
		break;
	case EEQUIP_SLOT::CHEST:
		SlotName = L"갑옷";
		break;
	case EEQUIP_SLOT::GAUNTLET:
		SlotName = L"장갑";
		break;
	case EEQUIP_SLOT::LEGGINGS:
		SlotName = L"각반";
		break;
	case EEQUIP_SLOT::ACCESSORIE_1:
		SlotName = L"악세사리 1";
		break;
	case EEQUIP_SLOT::ACCESSORIE_2:
		SlotName = L"악세사리 2";
		break;
	case EEQUIP_SLOT::ACCESSORIE_3:
		SlotName = L"악세사리 3";
		break;
	case EEQUIP_SLOT::ACCESSORIE_4:
		SlotName = L"악세사리 4";
		break;
	case EEQUIP_SLOT::CONSUMABLE_1:
		SlotName = L"소비아이템 1";
		break;
	case EEQUIP_SLOT::CONSUMABLE_2:
		SlotName = L"소비아이템 2";
		break;
	case EEQUIP_SLOT::CONSUMABLE_3:
		SlotName = L"소비아이템 3";
		break;
	case EEQUIP_SLOT::CONSUMABLE_4:
		SlotName = L"소비아이템 4";
		break;
	case EEQUIP_SLOT::CONSUMABLE_5:
		SlotName = L"소비아이템 5";
		break;
	default:
		break;
	}

	return SlotName;
}
