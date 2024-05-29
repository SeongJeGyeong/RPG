// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_QuickSlot.h"
#include "UI_Player_QuickSlotItem.h"
#include "../Manager/Equip_Mgr.h"
#include "Components/ProgressBar.h"

void UUI_Player_QuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	m_UpperSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_UpperSlot"));
	m_LowerSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_LowerSlot"));
	m_NextQuickSlotItem = Cast<UUI_Player_QuickSlotItem>(GetWidgetFromName("UI_NextQuickSlot"));
	m_LowerSlotDelayBar = Cast<UProgressBar>(GetWidgetFromName("LowerSlot_DelayBar"));

	if (!IsValid(m_UpperSlotItem) || !IsValid(m_LowerSlotItem) || !IsValid(m_NextQuickSlotItem) || !IsValid(m_LowerSlotDelayBar))
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

void UUI_Player_QuickSlot::SetQuickSlotOpacity(float Alpha, bool UorL)
{
	if (UorL)
	{
		m_UpperSlotItem->SetRenderOpacity(Alpha);
		//m_UpperSlotItem->SetColorAndOpacity(FLinearColor::FLinearColor(R, G, B, A));
	}
	else
	{
		m_LowerSlotItem->SetRenderOpacity(Alpha);
		//m_LowerSlotItem->SetColorAndOpacity(FLinearColor::FLinearColor(R, G, B, A));
	}
}

void UUI_Player_QuickSlot::SetLowerSlotDelay(float _DelayPercent)
{	
	m_LowerSlotDelayBar->SetPercent(_DelayPercent);
}
