// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InvenItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "../Item/Item_InvenData.h"
#include "UI_Inventory.h"
#include "Components/MenuAnchor.h"

void UUI_InvenItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemImg")));
	m_EquipMark = Cast<UImage>(GetWidgetFromName(TEXT("EquipMark")));
	m_ItemQnt = Cast<UTextBlock>(GetWidgetFromName(TEXT("Quantity")));
	m_ItemBtn = Cast<UButton>(GetWidgetFromName(TEXT("ItemBtn")));
	m_MenuAnchor = Cast<UMenuAnchor>(GetWidgetFromName(TEXT("ItemMenuAnchor")));
	
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
}

void UUI_InvenItem::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_InvenItem::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	InitFromData(ListItemObject);
}

void UUI_InvenItem::InitFromData(UObject* _Data)
{
	m_ItemData = Cast<UItem_InvenData>(_Data);

	if (!IsValid(m_ItemData))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 위젯에 입력된 아이템 데이터가 올바르지 않음"));
	}

	// 아이템 이미지 세팅
	FString ItemImgPath = m_ItemData->GetItemImgPath();
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);

	// 아이템 수량 세팅
	m_ItemQnt->SetText(FText::FromString(FString::Printf(TEXT("%d"), m_ItemData->GetItemQnt())));

	if (m_ItemData->GetEquiped() != EEQUIP_SLOT::EMPTY)
	{
		m_EquipMark->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUI_InvenItem::ItemBtnClicked()
{
	if (bAnchorActive)
	{
		m_MenuAnchor->Open(true);
	}
	else
	{
		m_EquipMark->SetVisibility(ESlateVisibility::Visible);
	}
}