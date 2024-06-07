// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_QuickSlot.h"
#include "UI_Player_QuickSlotItem.h"
#include "../Manager/Equip_Mgr.h"
#include "Components/ProgressBar.h"

void UUI_Player_QuickSlot::NativeConstruct()
{
	if (!IsValid(m_UI_UpperSlot) || !IsValid(m_UI_LowerSlot) || !IsValid(m_UI_NextQuickSlot) || !IsValid(m_LowerSlot_DelayBar))
	{
		UE_LOG(LogTemp, Error, TEXT("퀵슬롯 UI 캐스팅 실패"));
	}
	else
	{
		RenewLowerQuickSlot(0);
	}

	Super::NativeConstruct();
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
		m_UI_LowerSlot->RenewQuickSlotItem(nullptr);
		m_UI_NextQuickSlot->RenewNextQuickSlotItem(nullptr);
		return;
	}
	
	FInvenItemRow* ItemData = UEquip_Mgr::GetInst(GetWorld())->GetQSItemForIndex(_Idx);
	m_UI_LowerSlot->RenewQuickSlotItem(ItemData);
	UEquip_Mgr::GetInst(GetWorld())->SetCurrentIndex(_Idx);

	// 퀵슬롯에 등록된 아이템이 하나뿐일 때
	int32 NextIdx = UEquip_Mgr::GetInst(GetWorld())->GetNextArrayIndex();
	if(_Idx == NextIdx)
	{
		m_UI_NextQuickSlot->RenewNextQuickSlotItem(nullptr);
	}
	// 퀵슬롯에 아이템이 두 개 이상일 때
	else
	{
		ItemData = UEquip_Mgr::GetInst(GetWorld())->GetQSItemForIndex(NextIdx);
		m_UI_NextQuickSlot->RenewNextQuickSlotItem(ItemData);
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
	m_UI_NextQuickSlot->RenewNextQuickSlotItem(ItemData);
}

void UUI_Player_QuickSlot::SetQuickSlotOpacity(float Alpha, bool UorL)
{
	if (UorL)
	{
		m_UI_UpperSlot->SetRenderOpacity(Alpha);
	}
	else
	{
		m_UI_LowerSlot->SetRenderOpacity(Alpha);
	}
}

void UUI_Player_QuickSlot::SetLowerSlotDelay(float _DelayPercent)
{	
	m_LowerSlot_DelayBar->SetPercent(_DelayPercent);
}
