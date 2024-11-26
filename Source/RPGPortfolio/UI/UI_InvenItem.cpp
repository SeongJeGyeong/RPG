// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InvenItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/MenuAnchor.h"
#include "../Item/Item_InvenData.h"
#include "UI_Inventory.h"
#include "UI_PlayerStat.h"
#include "UI_ItemSelectMenu.h"
#include "../Characters/Player_Base_Knight.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Manager/GISubsystem_InvenMgr.h"

void UUI_InvenItem::NativeConstruct()
{	
	if (!IsValid(m_ItemImg) || !IsValid(m_ItemQnt) || !IsValid(m_EquipMark))
	{
		UE_LOG(LogTemp, Error, TEXT("InvenItem의 하위 위젯을 찾지 못함"));
	}
	else
	{
		m_EquipMark->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_ItemBtn))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 버튼 위젯을 찾지 못함"));
	}
	m_ItemBtn->OnClicked.AddDynamic(this, &UUI_InvenItem::ItemBtnClicked);

	m_ItemMenuAnchor->OnGetUserMenuContentEvent.BindUFunction(this, FName("MenuAnchorDataSetting"));

	Super::NativeConstruct();
}

void UUI_InvenItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	InitFromData(ListItemObject);
}

void UUI_InvenItem::InitFromData(UObject* _Data)
{
	UItem_InvenData* pItem = Cast<UItem_InvenData>(_Data);
	if (!IsValid(pItem))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 위젯에 입력된 아이템 데이터가 올바르지 않음"));
	}

	eID = pItem->GetItemID();
	eSelectedSlot = pItem->GetEquiped();
	eType = pItem->GetItemType();
	// 아이템 이미지 세팅
	FString ItemImgPath = pItem->GetItemImgPath();
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);

	// 아이템 수량 세팅
	m_ItemQnt->SetText(FText::FromString(FString::Printf(TEXT("%d"), pItem->GetItemQnt())));

	if ( pItem->GetEquiped() == EEQUIP_SLOT::EMPTY)
	{
		m_EquipMark->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_EquipMark->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_InvenItem::ItemBtnClicked()
{
	// 인벤토리에서 아이템 클릭 시
	if (bAnchorActive)
	{
		APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		bItemUseDelay = pPlayer->GetbItemDelay();
		m_ItemMenuAnchor->Open(true);
		PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
	}
	// 장비 아이템 선택창에서 아이템 클릭 시
	else
	{
		UGISubsystem_InvenMgr* pInvenMgr = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();

		if ( IsValid(pInvenMgr) )
		{
			FInvenItemRow* pItemRow = pInvenMgr->GetInvenItemInfo(eID);
			if ( pItemRow->EquipedSlot == eSelectedSlot )
			{
				PlaySound(GETMENUSOUND(EMenuSound::ITEM_UNEQUIP));
			}
			else
			{
				PlaySound(GETMENUSOUND(EMenuSound::ITEM_EQUIP));
			}
			pInvenMgr->ChangeEquipItem(pItemRow->ID, eSelectedSlot);

			FGameItemInfo* pInfo = pInvenMgr->GetItemInfo(pItemRow->ID);
			// 무기 및 방어구 교체 시
			if ( pInfo->Type == EITEM_TYPE::WEAPON || pInfo->Type == EITEM_TYPE::ARM_HELM ||
				pInfo->Type == EITEM_TYPE::ARM_CHEST || pInfo->Type == EITEM_TYPE::ARM_GAUNTLET ||
				pInfo->Type == EITEM_TYPE::ARM_LEGGINGS )
			{
				m_StatUI->RenewBasePower();
			}
		}
	}
}

UUserWidget* UUI_InvenItem::MenuAnchorDataSetting()
{
	UUI_ItemSelectMenu* pWidget = Cast<UUI_ItemSelectMenu>(CreateWidget(GetWorld(), m_ItemMenuAnchor->MenuClass, FName("Anchor")));
	pWidget->SetbItemUseDelay(bItemUseDelay);

	pWidget->SetSelectedItemType(eType);
	pWidget->SetSelectedItemID(eID);
	pWidget->SetSelectedItemSlot(eSelectedSlot);

	return pWidget;
}