// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Base.h"
#include "UI_Player_Main.h"
#include "UI_Player_QuickSlot.h"
#include "UI_Player_Soul.h"
#include "UI_Menu_Main.h"
#include "UI_Boss.h"
#include "UI_Message_Main.h"
#include "UI_Message_Item.h"
#include "Components/Image.h"
#include "../Manager/GISubsystem_SoundMgr.h"

void UUI_Base::NativeConstruct()
{
	if(!IsValid(m_UI_MenuMain))
	{
		UE_LOG(LogTemp, Error, TEXT("메뉴 UI 로드 실패"));
	}
	else
	{
		m_UI_MenuMain->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (!IsValid(m_UI_Boss))
	{
		UE_LOG(LogTemp, Error, TEXT("보스 UI 로드 실패"));
	}
	else
	{
		m_UI_Boss->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (!IsValid(m_UI_MessageBox_Main))
	{
		UE_LOG(LogTemp, Error, TEXT("메인 메시지박스 UI 로드 실패"));
	}
	else
	{
		m_UI_MessageBox_Main->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (!IsValid(m_UI_MessageBox_Item))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 메시지박스 UI 로드 실패"));
	}
	else
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(m_UI_PlayerBar))
	{
		m_UI_PlayerBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if ( IsValid(m_UI_QuickSlotMain) )
	{
		m_UI_QuickSlotMain->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	if ( IsValid(m_UI_Soul) )
	{
		m_UI_Soul->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	Super::NativeConstruct();
}

void UUI_Base::MenuVisibility(ESlateVisibility _Visibility)
{
	m_UI_MenuMain->SetVisibility(_Visibility);
}

bool UUI_Base::IsOpendMenu()
{
	if (m_UI_MenuMain->GetVisibility() == ESlateVisibility::Visible || m_UI_MenuMain->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
	{
		return true;
	}

	return false;
}

void UUI_Base::SetMainMessageUI(FText _Command, FText _Action)
{
	m_UI_MessageBox_Main->SetMessageText(_Command, _Action);
}

void UUI_Base::ShowMainMessageUI(bool _bShow)
{
	if (_bShow)
	{
		m_UI_MessageBox_Main->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		if (!bDisplayRootMessage)
		{
			m_UI_MessageBox_Main->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUI_Base::ShowItemMessageUI(bool _bShow)
{
	bDisplayRootMessage = _bShow;
	if ( _bShow )
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUI_Base::RenewAmountSoul(int32 _GainedSoul)
{
	m_UI_Soul->RenewAmountOfSoul(_GainedSoul);
}

void UUI_Base::RenewQuickSlotUI(int32 _idx)
{
	m_UI_QuickSlotMain->RenewLowerQuickSlot(_idx);
}

void UUI_Base::SetQuickSlotUIOpacity(float _alpha, bool _UorL)
{
	m_UI_QuickSlotMain->SetQuickSlotOpacity(_alpha, _UorL);
}

void UUI_Base::SetQuickSlotUIDelay(float _DelayPercnet)
{
	m_UI_QuickSlotMain->SetLowerSlotDelay(_DelayPercnet);
}
