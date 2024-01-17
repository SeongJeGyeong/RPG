// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InvenItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"

void UUI_InvenItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemImg")));
	m_ItemQnt = Cast<UTextBlock>(GetWidgetFromName(TEXT("Quantity")));

	if (!IsValid(m_ItemImg) || !IsValid(m_ItemQnt))
	{
		UE_LOG(LogTemp, Error, TEXT("InvenItem의 하위 위젯을 찾지 못함"));
	}
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
	UItem_InvenData* pItemData = Cast<UItem_InvenData>(_Data);

	if (!IsValid(pItemData))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 위젯에 입력된 아이템 데이터가 올바르지 않음"));
	}

	// 아이템 이미지 세팅
	FString ItemImgPath = pItemData->GetItemImgPath();
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);

	// 아이템 수량 세팅
	m_ItemQnt->SetText(FText::FromString(FString::Printf(TEXT("%d"), pItemData->GetItemQnt())));
}
