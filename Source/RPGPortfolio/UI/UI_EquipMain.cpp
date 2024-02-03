// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipMain.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "UI_EquipItem.h"
#include "UI_EquipItemList.h"
#include "../System/PlayerState_Base.h"
#include "Kismet/GameplayStatics.h"

void UUI_EquipMain::NativeConstruct()
{
	Super::NativeConstruct();

	m_SelItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Sel_ItemName")));
	m_SelItemSlot = Cast<UTextBlock>(GetWidgetFromName(TEXT("Sel_ItemSlot")));
	m_Tooltip = Cast<UUI_ItemTooltip>(GetWidgetFromName(TEXT("ItemTooltipUI")));
	m_Stat = Cast<UUI_PlayerStat>(GetWidgetFromName(TEXT("PlayerStatUI")));
	m_ItemList = Cast<UUI_EquipItemList>(GetWidgetFromName(TEXT("ItemListUI")));
	
	if ( !IsValid(m_SelItemName) || !IsValid(m_SelItemSlot) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 텍스트 UI 캐스팅 실패"));
	}

	if ( !IsValid(m_Tooltip) || !IsValid(m_Stat) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 서브 UI 캐스팅 실패"));
	}
	else
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		if ( !IsValid(pPlayerState) )
		{
			UE_LOG(LogTemp, Error, TEXT("플레이어스테이트 획득 실패"));
		}
		else
		{
			m_Stat->SetPlayerStatUI(Cast<APlayerState_Base>(pPlayerState));
		}
	}

	if ( !IsValid(m_ItemList) )
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 리스트 UI 캐스팅 실패"));
	}
	else
	{
		m_ItemList->SetVisibility(ESlateVisibility::Hidden);
		OnNativeVisibilityChanged.AddUObject(this, &UUI_EquipMain::ListVisibilityChanged);
	}

	m_Weapon_1 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Weapon_1")));
	m_Weapon_2 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Weapon_2")));
	m_Weapon_3 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Weapon_3")));
	m_Shield_1 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Shield_1")));
	m_Shield_2 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Shield_2")));
	m_Shield_3 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Shield_3")));
	m_Arrow = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Arrow")));
	m_Bolt = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Bolt")));
	m_Helm = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Helm")));
	m_Chest = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Chest")));
	m_Gauntlet = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Gauntlet")));
	m_Leggings = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Leggings")));
	m_Accessorie_1 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_1")));
	m_Accessorie_2 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_2")));
	m_Accessorie_3 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_3")));
	m_Accessorie_4 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Accessorie_4")));
	m_Consumable_1 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_1")));
	m_Consumable_2 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_2")));
	m_Consumable_3 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_3")));
	m_Consumable_4 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_4")));
	m_Consumable_5 = Cast<UUI_EquipItem>(GetWidgetFromName(TEXT("Consumable_5")));

	if (!IsValid(m_Weapon_1) || !IsValid(m_Weapon_2) || !IsValid(m_Weapon_3) || !IsValid(m_Shield_1) || !IsValid(m_Shield_2) || !IsValid(m_Shield_3) ||
		!IsValid(m_Arrow) || !IsValid(m_Bolt) || !IsValid(m_Helm) || !IsValid(m_Chest) || !IsValid(m_Gauntlet) || !IsValid(m_Leggings) ||
		!IsValid(m_Accessorie_1) || !IsValid(m_Accessorie_2) || !IsValid(m_Accessorie_3) || !IsValid(m_Accessorie_4) || 
		!IsValid(m_Consumable_1) || !IsValid(m_Consumable_2) || !IsValid(m_Consumable_3) || !IsValid(m_Consumable_4) || !IsValid(m_Consumable_5))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템슬롯 캐스팅 실패"));
	}
	else
	{
		InitEquipSlot(m_Weapon_1);
		m_Weapon_1->SetSlotType(EEQUIP_SLOT::WEAPON_1);
		InitEquipSlot(m_Weapon_2);
		m_Weapon_2->SetSlotType(EEQUIP_SLOT::WEAPON_2);
		InitEquipSlot(m_Weapon_3);
		m_Weapon_3->SetSlotType(EEQUIP_SLOT::WEAPON_3);
		InitEquipSlot(m_Shield_1);
		m_Shield_1->SetSlotType(EEQUIP_SLOT::SHIELD_1);
		InitEquipSlot(m_Shield_2);
		m_Shield_2->SetSlotType(EEQUIP_SLOT::SHIELD_2);
		InitEquipSlot(m_Shield_3);
		m_Shield_3->SetSlotType(EEQUIP_SLOT::SHIELD_3);
		InitEquipSlot(m_Arrow);
		m_Arrow->SetSlotType(EEQUIP_SLOT::ARROW);
		InitEquipSlot(m_Bolt);
		m_Bolt->SetSlotType(EEQUIP_SLOT::BOLT);
		InitEquipSlot(m_Helm);
		m_Helm->SetSlotType(EEQUIP_SLOT::HELM);
		InitEquipSlot(m_Chest);
		m_Chest->SetSlotType(EEQUIP_SLOT::CHEST);
		InitEquipSlot(m_Gauntlet);
		m_Gauntlet->SetSlotType(EEQUIP_SLOT::GAUNTLET);
		InitEquipSlot(m_Leggings);
		m_Leggings->SetSlotType(EEQUIP_SLOT::LEGGINGS);
		InitEquipSlot(m_Accessorie_1);
		m_Accessorie_1->SetSlotType(EEQUIP_SLOT::ACCESSORIE_1);
		InitEquipSlot(m_Accessorie_2);
		m_Accessorie_2->SetSlotType(EEQUIP_SLOT::ACCESSORIE_2);
		InitEquipSlot(m_Accessorie_3);
		m_Accessorie_3->SetSlotType(EEQUIP_SLOT::ACCESSORIE_3);
		InitEquipSlot(m_Accessorie_4);
		m_Accessorie_4->SetSlotType(EEQUIP_SLOT::ACCESSORIE_4);
		InitEquipSlot(m_Consumable_1);
		m_Consumable_1->SetSlotType(EEQUIP_SLOT::CONSUMABLE_1);
		InitEquipSlot(m_Consumable_2);
		m_Consumable_2->SetSlotType(EEQUIP_SLOT::CONSUMABLE_2);
		InitEquipSlot(m_Consumable_3);
		m_Consumable_3->SetSlotType(EEQUIP_SLOT::CONSUMABLE_3);
		InitEquipSlot(m_Consumable_4);
		m_Consumable_4->SetSlotType(EEQUIP_SLOT::CONSUMABLE_4);
		InitEquipSlot(m_Consumable_5);
		m_Consumable_5->SetSlotType(EEQUIP_SLOT::CONSUMABLE_5);
	}

}

void UUI_EquipMain::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_EquipMain::ListVisibilityChanged(ESlateVisibility _Visibility)
{
	if (_Visibility == ESlateVisibility::Hidden)
	{
		m_ItemList->SetVisibility(ESlateVisibility::Hidden);
	}
}

//void UUI_EquipMain::ItemBtnClicked()
//{
//	UE_LOG(LogTemp, Warning, TEXT("장비창 버튼 클릭"));
//	RenewItemListUI()
//	m_ItemList->SetVisibility(ESlateVisibility::Visible);
//}

void UUI_EquipMain::RenewItemListUI(EITEM_TYPE _Type)
{
}

void UUI_EquipMain::InitEquipSlot(UUI_EquipItem* _SlotUI)
{
	_SlotUI->SetItemListUI(m_ItemList);
	_SlotUI->SetItemNameUI(m_SelItemName);
	_SlotUI->SetItemSlotUI(m_SelItemSlot);
}
