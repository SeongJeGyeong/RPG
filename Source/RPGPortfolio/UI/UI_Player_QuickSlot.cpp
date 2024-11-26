// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_QuickSlot.h"
#include "UI_Player_QuickSlotItem.h"
#include "Components/ProgressBar.h"
#include "../Item/Item_InvenData.h"

void UUI_Player_QuickSlot::NativeConstruct()
{
	if (!IsValid(m_UI_UpperSlot) || !IsValid(m_UI_LowerSlot) || !IsValid(m_UI_NextQuickSlot) || !IsValid(m_LowerSlot_DelayBar))
	{
		UE_LOG(LogTemp, Error, TEXT("퀵슬롯 UI 캐스팅 실패"));
	}

	Super::NativeConstruct();
}

void UUI_Player_QuickSlot::InitLowerQuickSlot(UItem_InvenData* _CurItem, UItem_InvenData* _NextItem)
{
	m_UI_LowerSlot->RenewQuickSlotItem(_CurItem);
	m_UI_NextQuickSlot->RenewNextQuickSlotItem(_NextItem);
}

void UUI_Player_QuickSlot::RenewLowerQuickSlot(UItem_InvenData* _InvenItem)
{
	m_UI_LowerSlot->RenewQuickSlotItem(_InvenItem);

	UWidgetBlueprintGeneratedClass* pWidgetClass = GetWidgetTreeOwningClass();
	for ( int32 i = 0; i < pWidgetClass->Animations.Num(); ++i )
	{
		if ( pWidgetClass->Animations[ i ].GetName() == TEXT("LowerSlotChangeAnim_INST") )
		{
			PlayAnimation(pWidgetClass->Animations[ i ]);
			break;
		}
	}
}

void UUI_Player_QuickSlot::RenewNextLowerQuickSlot(UItem_InvenData* _InvenItem)
{
	m_UI_NextQuickSlot->RenewNextQuickSlotItem(_InvenItem);
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
