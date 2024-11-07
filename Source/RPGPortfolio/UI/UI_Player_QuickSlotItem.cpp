// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Player_QuickSlotItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/GISubsystem_InvenMgr.h"

void UUI_Player_QuickSlotItem::NativeConstruct()
{
	if (!IsValid(m_Dish) || !IsValid(m_ItemImg) || !IsValid(m_ItemName) || !IsValid(m_Amount))
	{
		UE_LOG(LogTemp, Error, TEXT("퀵슬롯 아이템 UI 캐스팅 실패"));
	}
	else
	{
		m_Dish->SetVisibility(ESlateVisibility::Hidden);
		m_ItemImg->SetVisibility(ESlateVisibility::Hidden);
	}

	Super::NativeConstruct();
}

void UUI_Player_QuickSlotItem::RenewQuickSlotItem(const FInvenItemRow* const _InvenItem)
{
	if (_InvenItem == nullptr)
	{
		m_Dish->SetVisibility(ESlateVisibility::Hidden);
		m_ItemImg->SetVisibility(ESlateVisibility::Hidden);
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_Amount->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FGameItemInfo* pInfo = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>()->GetItemInfo(_InvenItem->ID);
	FString ItemImgPath = pInfo->IconImgPath;
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);
	m_ItemName->SetText(FText::FromString(pInfo->ItemName));
	m_Amount->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenItem->Stack)));
	m_Dish->SetVisibility(ESlateVisibility::HitTestInvisible);
	m_ItemImg->SetVisibility(ESlateVisibility::HitTestInvisible);
	m_ItemName->SetVisibility(ESlateVisibility::HitTestInvisible);
	m_Amount->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUI_Player_QuickSlotItem::RenewNextQuickSlotItem(const FInvenItemRow* const _InvenItem)
{
	if (_InvenItem == nullptr)
	{
		m_Dish->SetVisibility(ESlateVisibility::Hidden);
		m_ItemImg->SetVisibility(ESlateVisibility::Hidden);
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_Amount->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FGameItemInfo* pInfo = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>()->GetItemInfo(_InvenItem->ID);
	FString ItemImgPath = pInfo->IconImgPath;
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);
	m_Dish->SetVisibility(ESlateVisibility::HitTestInvisible);
	m_ItemImg->SetVisibility(ESlateVisibility::HitTestInvisible);
}
