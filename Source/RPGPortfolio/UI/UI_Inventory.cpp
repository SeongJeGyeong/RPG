// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "Components/TileView.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "../Item/Item_InvenData.h"
#include "../System/PlayerState_Base.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "../Header/Enum.h"
#include "../Manager/Inventory_Mgr.h"

void UUI_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemName = Cast<UTextBlock>(GetWidgetFromName(L"ItemName"));
	m_Category = Cast<UTextBlock>(GetWidgetFromName(L"CategoryText"));
	m_LeftBtn = Cast<UButton>(GetWidgetFromName(L"Btn_Cat_Left"));
	m_RightBtn = Cast<UButton>(GetWidgetFromName(L"Btn_Cat_Right"));
	m_TileView = Cast<UTileView>(GetWidgetFromName(L"ItemTileView"));
	m_Tooltip = Cast<UUI_ItemTooltip>(GetWidgetFromName(L"ItemTooltipUI"));
	m_Status = Cast<UUI_PlayerStat>(GetWidgetFromName(L"PlayerStatUI"));

	if ( !IsValid(m_ItemName) )
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 이름 위젯 찾지 못함"));
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_Category))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 텍스트 위젯 찾지 못함"));
	}

	if (!IsValid(m_LeftBtn) || !IsValid(m_RightBtn))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 버튼 위젯 찾지 못함"));
	}
	else
	{
		m_LeftBtn->OnClicked.AddDynamic(this, &UUI_Inventory::LeftBtnClicked);
		m_RightBtn->OnClicked.AddDynamic(this, &UUI_Inventory::RightBtnClicked);
	}

	if (!IsValid(m_TileView))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 타일 뷰 찾지 못함"));
	}
	else
	{
		m_TileView->OnItemIsHoveredChanged().AddUObject(this, &UUI_Inventory::OnTileHovered);
	}

	if (!IsValid(m_Tooltip))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 툴팁 위젯 찾지 못함"));
	}
	else
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_Status))
	{
		UE_LOG(LogTemp, Error, TEXT("캐릭터스탯 위젯 찾지 못함"));
	}

	eCategory = EITEM_TYPE::ALL;
	SetCategoryText(eCategory);
}

void UUI_Inventory::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Inventory::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
	if (_Hovered)
	{
		UItem_InvenData* pData = Cast<UItem_InvenData>(_ItemData);

		m_ItemName->SetText(FText::FromString(pData->GetItemName()));
		m_ItemName->SetVisibility(ESlateVisibility::Visible);

		m_Tooltip->SetTooltipUI(pData);
		m_Tooltip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
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
		SetCategoryText(eCategory);
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
		SetCategoryText(eCategory);
	}
}

void UUI_Inventory::AddItem(UObject* _ItemData)
{
	m_TileView->AddItem(_ItemData);
}

void UUI_Inventory::Clear()
{
	if (IsValid(m_TileView))
	{
		m_TileView->ClearListItems();
	}
}

bool UUI_Inventory::IsInventoryOpened()
{
	if (this->GetVisibility() == ESlateVisibility::Visible)
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
	m_Status->SetPlayerStatUI(Cast<APlayerState_Base>(_PlayerState));
}

void UUI_Inventory::SetCategoryText(EITEM_TYPE _Type)
{
	switch (_Type)
	{
	case EITEM_TYPE::ALL:
		m_Category->SetText(FText::FromString(L"전체아이템"));
		break;
	case EITEM_TYPE::CONSUMABLE:
		m_Category->SetText(FText::FromString(L"소비아이템"));
		break;
	case EITEM_TYPE::WEAPON:
		m_Category->SetText(FText::FromString(L"무기"));
		break;
	case EITEM_TYPE::ARMOR:
		m_Category->SetText(FText::FromString(L"방어구"));
		break;
	case EITEM_TYPE::ACCESSORIE:
		m_Category->SetText(FText::FromString(L"악세사리"));
		break;
	case EITEM_TYPE::ARROWS:
		m_Category->SetText(FText::FromString(L"화살"));
		break;
	case EITEM_TYPE::SPELL:
		m_Category->SetText(FText::FromString(L"마법"));
		break;
	case EITEM_TYPE::KEY:
		m_Category->SetText(FText::FromString(L"중요아이템"));
		break;
	case EITEM_TYPE::MISC:
		m_Category->SetText(FText::FromString(L"기타아이템"));
		break;
	default:
		break;
	}
}
