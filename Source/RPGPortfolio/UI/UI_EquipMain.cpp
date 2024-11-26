// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipMain.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "UI_EquipItem.h"
#include "UI_EquipItemList.h"
#include "Kismet/GameplayStatics.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/GISubsystem_InvenMgr.h"

void UUI_EquipMain::NativeConstruct()
{	
	if ( !IsValid(m_Sel_ItemName) || !IsValid(m_Sel_ItemSlot) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 텍스트 UI 캐스팅 실패"));
	}

	if ( !IsValid(m_ItemTooltipUI) || !IsValid(m_Equip_StatUI) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 서브 UI 캐스팅 실패"));
	}
	else
	{
		m_ItemTooltipUI->SetVisibility(ESlateVisibility::Hidden);
	}

	if ( !IsValid(m_Equip_ItemListUI) )
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 리스트 UI 캐스팅 실패"));
	}
	else
	{
		m_Equip_ItemListUI->SetVisibility(ESlateVisibility::Collapsed);
		OnNativeVisibilityChanged.AddUObject(this, &UUI_EquipMain::ListVisibilityChanged);
	}

	Super::NativeConstruct();
}

void UUI_EquipMain::ListVisibilityChanged(ESlateVisibility _Visibility)
{
	if (_Visibility == ESlateVisibility::Collapsed)
	{
		m_Equip_ItemListUI->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		m_Equip_StatUI->SetPlayerStatUI();
	}
}

void UUI_EquipMain::BindInvenMgr()
{
	UGISubsystem_InvenMgr* InvenManager = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
	if ( InvenManager )
	{
		InvenManager->OnRenewEquipItem.AddUObject(this, &UUI_EquipMain::RenewEquipItem);
		InvenManager->OnClearEquipList.AddUObject(this, &UUI_EquipMain::ClearEquipItemList);
		InvenManager->OnAddEquipItemList.AddUObject(this, &UUI_EquipMain::AddItemInEquipList);
	}
}

bool UUI_EquipMain::GetItemListVisibility()
{
	return m_Equip_ItemListUI->GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}

void UUI_EquipMain::CloseItemList()
{
	m_Equip_ItemListUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_EquipMain::ClearEquipItemList()
{
	m_Equip_ItemListUI->ClearTileView();
}

void UUI_EquipMain::AddItemInEquipList(UObject* _Data)
{
	m_Equip_ItemListUI->AddEquipItemList(_Data);
}

void UUI_EquipMain::RenewEquipItem(EEQUIP_SLOT _Slot, UItem_InvenData* _ItemData)
{
	UE_LOG(LogTemp, Warning, TEXT("RenewEquipItem"));
	UUI_EquipItem* pEquipItem = nullptr;

	switch (_Slot)
	{
	case EEQUIP_SLOT::WEAPON_1:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Weapon_1")));
		break;
	case EEQUIP_SLOT::WEAPON_2:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Weapon_2")));
		break;
	case EEQUIP_SLOT::WEAPON_3:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Weapon_3")));
		break;
	case EEQUIP_SLOT::SHIELD_1:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Shield_1")));
		break;
	case EEQUIP_SLOT::SHIELD_2:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Shield_2")));
		break;
	case EEQUIP_SLOT::SHIELD_3:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Shield_3")));
		break;
	case EEQUIP_SLOT::ARROW:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Arrow")));
		break;
	case EEQUIP_SLOT::BOLT:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Bolt")));
		break;
	case EEQUIP_SLOT::HELM:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Helm")));
		break;
	case EEQUIP_SLOT::CHEST:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Chest")));
		break;
	case EEQUIP_SLOT::GAUNTLET:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Gauntlet")));
		break;
	case EEQUIP_SLOT::LEGGINGS:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Leggings")));
		break;
	case EEQUIP_SLOT::ACCESSORIE_1:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_1")));
		break;
	case EEQUIP_SLOT::ACCESSORIE_2:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_2")));
		break;
	case EEQUIP_SLOT::ACCESSORIE_3:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_3")));
		break;
	case EEQUIP_SLOT::ACCESSORIE_4:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_4")));
		break;
	case EEQUIP_SLOT::CONSUMABLE_1:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_1")));
		break;
	case EEQUIP_SLOT::CONSUMABLE_2:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_2")));
		break;
	case EEQUIP_SLOT::CONSUMABLE_3:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_3")));
		break;
	case EEQUIP_SLOT::CONSUMABLE_4:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_4")));
		break;
	case EEQUIP_SLOT::CONSUMABLE_5:
		pEquipItem = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_5")));
		break;
	default:
		break;
	}
	if ( pEquipItem == nullptr )
	{
		UE_LOG(LogTemp, Error, TEXT("RenewEquipItem : 위젯 할당 실패"));
		return;
	}
	pEquipItem->SetEquipItem(_ItemData);
}