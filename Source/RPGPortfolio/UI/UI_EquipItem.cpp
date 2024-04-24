// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI_EquipItemList.h"
#include "UI_ItemTooltip.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"
#include "../System/DataAsset/DA_MenuSound.h"

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

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if ( !IsValid(m_Sound) )
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 사운드 로드 실패"));
	}
}

void UUI_EquipItem::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_EquipItem::SetEquipItem(UItem_InvenData* _ItemData)
{
	m_ItemData = _ItemData;
	//m_ItemData = MakeShareable(_ItemData);
	//m_ItemData.SetAllItemData(*_ItemData);
	//m_ItemData.IsValidLowLevel()
	if (!IsValid(m_ItemData))
	{
		m_ItemImg->SetVisibility(ESlateVisibility::Hidden);
		m_DishImg->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		FString ItemImgPath = _ItemData->GetItemImgPath();
		UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
		m_ItemImg->SetBrushFromTexture(pTex2D);
		m_ItemImg->SetVisibility(ESlateVisibility::Visible);
		m_DishImg->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUI_EquipItem::SetEquipItemStack(const uint16& _Stack)
{
	m_ItemData->SetItemQnt(_Stack);
}

void UUI_EquipItem::ItemBtnClicked()
{
	ItemList->SetSlotCategory(eSlotType);
	switch ( eSlotType )
	{
	case EEQUIP_SLOT::WEAPON_1:
		ItemList->SetCategoryText(FText::FromString(L"무기 1"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::WEAPON);
		break;
	case EEQUIP_SLOT::WEAPON_2:
		ItemList->SetCategoryText(FText::FromString(L"무기 2"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::WEAPON);
		break;
	case EEQUIP_SLOT::WEAPON_3:
		ItemList->SetCategoryText(FText::FromString(L"무기 3"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::WEAPON);
		break;
	case EEQUIP_SLOT::SHIELD_1:
		ItemList->SetCategoryText(FText::FromString(L"방패 1"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::SHIELD);
		break;
	case EEQUIP_SLOT::SHIELD_2:
		ItemList->SetCategoryText(FText::FromString(L"방패 2"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::SHIELD);
		break;
	case EEQUIP_SLOT::SHIELD_3:
		ItemList->SetCategoryText(FText::FromString(L"방패 3"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::SHIELD);
		break;
	case EEQUIP_SLOT::ARROW:
		ItemList->SetCategoryText(FText::FromString(L"화살"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ARROWS);
		break;
	case EEQUIP_SLOT::BOLT:
		ItemList->SetCategoryText(FText::FromString(L"볼트"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ARROWS);
		break;
	case EEQUIP_SLOT::HELM:
		ItemList->SetCategoryText(FText::FromString(L"투구"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ARM_HELM);
		break;
	case EEQUIP_SLOT::CHEST:
		ItemList->SetCategoryText(FText::FromString(L"갑옷"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ARM_CHEST);
		break;
	case EEQUIP_SLOT::GAUNTLET:
		ItemList->SetCategoryText(FText::FromString(L"장갑"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ARM_GAUNTLET);
		break;
	case EEQUIP_SLOT::LEGGINGS:
		ItemList->SetCategoryText(FText::FromString(L"각반"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ARM_LEGGINGS);
		break;
	case EEQUIP_SLOT::ACCESSORIE_1:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 1"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ACCESSORIE);
		break;
	case EEQUIP_SLOT::ACCESSORIE_2:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 2"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ACCESSORIE);
		break;
	case EEQUIP_SLOT::ACCESSORIE_3:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 3"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ACCESSORIE);
		break;
	case EEQUIP_SLOT::ACCESSORIE_4:
		ItemList->SetCategoryText(FText::FromString(L"악세사리 4"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::ACCESSORIE);
		break;
	case EEQUIP_SLOT::CONSUMABLE_1:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 1"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::CONSUMABLE);
		break;
	case EEQUIP_SLOT::CONSUMABLE_2:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 2"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::CONSUMABLE);
		break;
	case EEQUIP_SLOT::CONSUMABLE_3:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 3"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::CONSUMABLE);
		break;
	case EEQUIP_SLOT::CONSUMABLE_4:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 4"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::CONSUMABLE);
		break;
	case EEQUIP_SLOT::CONSUMABLE_5:
		ItemList->SetCategoryText(FText::FromString(L"소비아이템 5"));
		UInventory_Mgr::GetInst(GetWorld())->RenewItemListUI(EITEM_TYPE::CONSUMABLE);
		break;
	default:
		break;
	}

	ItemList->SetVisibility(ESlateVisibility::Visible);
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_OPEN));
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
		FString sItemName = m_ItemData->GetItemName();
		UE_LOG(LogTemp, Warning, TEXT("아이템이름 : %s"), *sItemName);
		ItemNameText->SetText(FText::FromString(sItemName));
		m_Tooltip->SetTooltipUI(m_ItemData);
		m_Tooltip->SetVisibility(ESlateVisibility::Visible);
	}

	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
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
	if (m_Tooltip->GetVisibility() == ESlateVisibility::Visible)
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
	}
}
