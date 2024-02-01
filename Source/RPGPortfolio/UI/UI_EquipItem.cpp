// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"

void UUI_EquipItem::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemBtn = Cast<UButton>(GetWidgetFromName(TEXT("ItemBtn")));
	m_ItemImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemImg")));
	m_DishImg = Cast<UImage>(GetWidgetFromName(TEXT("DishImg")));

	if (!IsValid(m_ItemBtn) || !IsValid(m_ItemBtn) || !IsValid(m_ItemBtn) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 아이템 캐스팅 실패"));
	}

}

void UUI_EquipItem::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}
