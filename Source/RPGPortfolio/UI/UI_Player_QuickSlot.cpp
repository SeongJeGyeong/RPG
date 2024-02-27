// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_QuickSlot.h"
#include "UI_Player_QuickSlotItem.h"
#include "../Manager/Equip_Mgr.h"

void UUI_Player_QuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	m_UpperSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_ItemSlot1"));
	m_LowerSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_ItemSlot2"));

	if (!IsValid(m_UpperSlotItem) || !IsValid(m_LowerSlotItem))
	{
		UE_LOG(LogTemp, Error, TEXT("퀵슬롯 UI 캐스팅 실패"));
	}
	else
	{
		RenewLowerQuickSlot(0);
	}
}

void UUI_Player_QuickSlot::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Player_QuickSlot::RenewLowerQuickSlot(int32 _Idx)
{
	FInvenItemRow* ItemData = UEquip_Mgr::GetInst(GetWorld())->GetSlotForIndex(_Idx);
	if (ItemData == nullptr)
	{
		if(UEquip_Mgr::GetInst(GetWorld())->GetQuickSlotValid())
		{
			int32 NextIdx = UEquip_Mgr::GetInst(GetWorld())->GetNextArrayIndex();
			ItemData = UEquip_Mgr::GetInst(GetWorld())->GetSlotForIndex(NextIdx);
			UEquip_Mgr::GetInst(GetWorld())->SetCurrentIndex(NextIdx);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("하단 퀵슬롯의 지정된 인덱스에 아이템 설정되지 않음"));
		}

		m_LowerSlotItem->RenewQuickSlotItem(ItemData);
		return;
	}
	m_LowerSlotItem->RenewQuickSlotItem(ItemData);
	UEquip_Mgr::GetInst(GetWorld())->SetCurrentIndex(_Idx);

	UWidgetBlueprintGeneratedClass* pWidgetClass = GetWidgetTreeOwningClass();
	for (int32 i = 0; i < pWidgetClass->Animations.Num(); ++i)
	{
		if (pWidgetClass->Animations[i].GetName() == TEXT("LowerSlotChangeAnim_INST"))
		{
			PlayAnimation(pWidgetClass->Animations[i]);
			break;
		}
	}
}
