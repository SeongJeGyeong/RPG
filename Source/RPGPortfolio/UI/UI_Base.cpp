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

void UUI_Base::NativeConstruct()
{
	if(!IsValid(m_UI_MenuMain))
	{
		UE_LOG(LogTemp, Error, TEXT("메뉴 UI 로드 실패"));
	}
	else
	{
		m_UI_MenuMain->SetVisibility(ESlateVisibility::Hidden);
	}
	if (!IsValid(m_UI_Boss))
	{
		UE_LOG(LogTemp, Error, TEXT("보스 UI 로드 실패"));
	}
	else
	{
		m_UI_Boss->SetVisibility(ESlateVisibility::Hidden);
	}
	if (!IsValid(m_UI_MessageBox_Main))
	{
		UE_LOG(LogTemp, Error, TEXT("메인 메시지박스 UI 로드 실패"));
	}
	else
	{
		m_UI_MessageBox_Main->SetVisibility(ESlateVisibility::Hidden);
	}
	if (!IsValid(m_UI_MessageBox_Item))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 메시지박스 UI 로드 실패"));
	}
	else
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::Hidden);
	}

	Super::NativeConstruct();
}

void UUI_Base::ShowMenu(bool _bShow)
{
	if (_bShow)
	{
		m_UI_MenuMain->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_UI_MenuMain->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_Base::ShowMainMessageUI(bool _bShow)
{
	if (_bShow)
	{
		m_UI_MessageBox_Main->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (!bDisplayRootMessage)
		{
			m_UI_MessageBox_Main->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUI_Base::ShowItemMessageUI(bool _bShow)
{
	bDisplayRootMessage = _bShow;
	if ( _bShow )
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::Hidden);
	}
}