// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_QuickSlot.h"
#include "UI_Player_QuickSlotItem.h"
#include "../Manager/Equip_Mgr.h"

void UUI_Player_QuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	m_UpperSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_UpperSlot"));
	m_LowerSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_LowerSlot"));
	m_NextQuickSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_NextQuickSlot"));

	if (!IsValid(m_UpperSlotItem) || !IsValid(m_LowerSlotItem) || !IsValid(m_NextQuickSlotItem))
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
	// 퀵슬롯에 등록된 아이템이 없을 때
	if (!UEquip_Mgr::GetInst(GetWorld())->QuickSlotValidForArr())
	{
		UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 아이템 없음"));
		m_LowerSlotItem->RenewQuickSlotItem(nullptr);
		m_NextQuickSlotItem->RenewNextQuickSlotItem(nullptr);
		return;
	}
	
	FInvenItemRow* ItemData = UEquip_Mgr::GetInst(GetWorld())->GetQSItemForIndex(_Idx);
	m_LowerSlotItem->RenewQuickSlotItem(ItemData);
	UEquip_Mgr::GetInst(GetWorld())->SetCurrentIndex(_Idx);

	// 퀵슬롯에 등록된 아이템이 하나뿐일 때
	int32 NextIdx = UEquip_Mgr::GetInst(GetWorld())->GetNextArrayIndex();
	if(_Idx == NextIdx)
	{
		m_NextQuickSlotItem->RenewNextQuickSlotItem(nullptr);
	}
	// 퀵슬롯에 아이템이 두 개 이상일 때
	else
	{
		ItemData = UEquip_Mgr::GetInst(GetWorld())->GetQSItemForIndex(NextIdx);
		m_NextQuickSlotItem->RenewNextQuickSlotItem(ItemData);
	}

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

void UUI_Player_QuickSlot::RenewNextQuickSlot(int32 _Idx)
{
	FInvenItemRow* ItemData = UEquip_Mgr::GetInst(GetWorld())->GetQSItemForIndex(_Idx);
	m_NextQuickSlotItem->RenewNextQuickSlotItem(ItemData);
}

void UUI_Player_QuickSlot::SetQuickSlotColor(float R, float G, float B, float A, bool UroL)
{
	// true면 위쪽 슬롯의 컬러 세팅
	// false면 아래쪽 슬롯 컬러 세팅
	if (UroL)
	{
		m_UpperSlotItem->SetColorAndOpacity(FLinearColor::FLinearColor(R, G, B, A));
	}
	else
	{
		m_LowerSlotItem->SetColorAndOpacity(FLinearColor::FLinearColor(R, G, B, A));
	}
}
