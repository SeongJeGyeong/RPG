// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_QuickSlot.h"
#include "UI_Player_QuickSlotItem.h"

void UUI_Player_QuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	m_UpperSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_ItemSlot1"));
	m_LowerSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_ItemSlot2"));

	if (!IsValid(m_UpperSlotItem) || !IsValid(m_LowerSlotItem))
	{
		UE_LOG(LogTemp, Error, TEXT("퀵슬롯 UI 캐스팅 실패"));
	}
}

void UUI_Player_QuickSlot::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}
