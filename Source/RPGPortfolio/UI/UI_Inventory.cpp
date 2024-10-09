// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "Components/TileView.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"
#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "UI_InvenItem.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Manager/GISubsystem_StatMgr.h"
#include "../System/DataAsset/DA_ItemCategoryIcon.h"

void UUI_Inventory::NativeConstruct()
{
	if ( !IsValid(m_ItemName) )
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 이름 위젯 찾지 못함"));
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_CategoryText))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 텍스트 위젯 찾지 못함"));
	}
	else
	{
		m_CategoryText->SetText(FText::FromString(L"전체아이템"));
	}
	if (!IsValid(m_Category_Img))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 이미지 위젯 찾지 못함"));
	}

	if (!IsValid(m_Btn_Cat_Left) || !IsValid(m_Btn_Cat_Right))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 버튼 위젯 찾지 못함"));
	}
	else
	{
		m_Btn_Cat_Left->OnClicked.AddDynamic(this, &UUI_Inventory::LeftBtnClicked);
		m_Btn_Cat_Right->OnClicked.AddDynamic(this, &UUI_Inventory::RightBtnClicked);
	}

	if (!IsValid(m_ItemTileView))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 타일 뷰 찾지 못함"));
	}
	else
	{
		m_ItemTileView->OnItemIsHoveredChanged().AddUObject(this, &UUI_Inventory::OnTileHovered);
	}

	if (!IsValid(m_ItemTooltipUI))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 툴팁 위젯 찾지 못함"));
	}
	else
	{
		m_ItemTooltipUI->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_PlayerStatUI))
	{
		UE_LOG(LogTemp, Error, TEXT("캐릭터스탯 위젯 찾지 못함"));
	}

	eCategory = EITEM_TYPE::ALL;
	SetCategoryUI(eCategory);

	Super::NativeConstruct();
}

void UUI_Inventory::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
	if (_Hovered)
	{
		// 인벤토리에서는 메뉴앵커 표시 되도록
		UUI_InvenItem* ItemUI = Cast<UUI_InvenItem>(m_ItemTileView->GetEntryWidgetFromItem(_ItemData));
		ItemUI->SetAnchorActive(true);
		UItem_InvenData* pData = Cast<UItem_InvenData>(_ItemData);
		FGameItemInfo* pItemInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(pData->GetItemID());
		m_ItemName->SetText(FText::FromString(pItemInfo->ItemName));
		m_ItemName->SetVisibility(ESlateVisibility::HitTestInvisible);

		m_ItemTooltipUI->SetTooltipUI(pData);
		m_ItemTooltipUI->SetVisibility(ESlateVisibility::HitTestInvisible);

		PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_ItemTooltipUI->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUI_Inventory::LeftBtnClicked()
{
	if (eCategory == EITEM_TYPE::ALL)
	{
		return;
	}
	else
	{
		int8 iCategory = static_cast<int8>(eCategory);
		eCategory = static_cast<EITEM_TYPE>(--iCategory);
		UInventory_Mgr::GetInst(GetWorld())->RenewInventoryUI(eCategory);
		SetCategoryUI(eCategory);

		PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));
	}
}

void UUI_Inventory::RightBtnClicked()
{
	if ( eCategory == EITEM_TYPE::MISC)
	{
		return;
	}
	else
	{
		int8 iCategory = static_cast<int8>( eCategory );
		eCategory = static_cast<EITEM_TYPE>(++iCategory);
		UInventory_Mgr::GetInst(GetWorld())->RenewInventoryUI(eCategory);
		SetCategoryUI(eCategory);

		PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));
	}
}

void UUI_Inventory::AddInventoryItem(UObject* _ItemData)
{
	m_ItemTileView->AddItem(_ItemData);
}

void UUI_Inventory::Clear()
{
	if (IsValid(m_ItemTileView))
	{
		m_ItemTileView->ClearListItems();
	}
}

bool UUI_Inventory::IsInventoryOpened()
{
	if (this->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UUI_Inventory::SetStatUI(APlayerState* _PlayerState)
{
	m_PlayerStatUI->SetPlayerStatUI();
}

void UUI_Inventory::SetCategoryUI(const EITEM_TYPE _Type)
{
	switch ( _Type )
	{
	case EITEM_TYPE::ALL:
		m_CategoryText->SetText(FText::FromString(L"전체아이템"));
		break;
	case EITEM_TYPE::CONSUMABLE:
		m_CategoryText->SetText(FText::FromString(L"소비아이템"));
		break;
	case EITEM_TYPE::WEAPON:
		m_CategoryText->SetText(FText::FromString(L"무기"));
		break;
	case EITEM_TYPE::SHIELD:
		m_CategoryText->SetText(FText::FromString(L"방패"));
		break;
	case EITEM_TYPE::ARM_HELM:
		m_CategoryText->SetText(FText::FromString(L"투구"));
		break;
	case EITEM_TYPE::ARM_CHEST:
		m_CategoryText->SetText(FText::FromString(L"갑옷"));
		break;
	case EITEM_TYPE::ARM_GAUNTLET:
		m_CategoryText->SetText(FText::FromString(L"장갑"));
		break;
	case EITEM_TYPE::ARM_LEGGINGS:
		m_CategoryText->SetText(FText::FromString(L"각반"));
		break;
	case EITEM_TYPE::ACCESSORIE:
		m_CategoryText->SetText(FText::FromString(L"악세사리"));
		break;
	case EITEM_TYPE::ARROWS:
		m_CategoryText->SetText(FText::FromString(L"화살"));
		break;
	case EITEM_TYPE::SPELL:
		m_CategoryText->SetText(FText::FromString(L"마법"));
		break;
	case EITEM_TYPE::KEY:
		m_CategoryText->SetText(FText::FromString(L"중요아이템"));
		break;
	case EITEM_TYPE::MISC:
		m_CategoryText->SetText(FText::FromString(L"기타아이템"));
		break;
	default:
		break;
	}

	UPaperSprite* pSprite = UInventory_Mgr::GetInst(GetWorld())->GetCategoryIcon(_Type);
	if ( IsValid(pSprite) )
	{
		m_Category_Img->SetBrushResourceObject(pSprite);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("카테고리 이미지 가져오기 실패"));
	}
}
