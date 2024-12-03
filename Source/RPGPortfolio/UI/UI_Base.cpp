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
#include "../Manager/GISubsystem_StatMgr.h"
#include "../Characters/Player_Base_Knight.h"
#include "../Manager/GISubsystem_InvenMgr.h"
#include "../Characters/Player_InvenComponent.h"

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

void UUI_Base::BindStatMgr()
{
	UGISubsystem_StatMgr* pStatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	if ( IsValid(pStatMgr) )
	{
		pStatMgr->OnRenewAmountSoul.AddUObject(this, &UUI_Base::RenewAmountSoul);
		pStatMgr->OnRenewHP.AddUObject(this, &UUI_Base::RenewUI_HP);
		pStatMgr->OnRenewMP.AddUObject(this, &UUI_Base::RenewUI_MP);
		pStatMgr->OnRenewST.AddUObject(this, &UUI_Base::RenewUI_ST);
	}
}

void UUI_Base::BindInvenMgr()
{
	UGISubsystem_InvenMgr* pInvenMgr = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
	if ( IsValid(pInvenMgr) )
	{
		pInvenMgr->OnRenewQS.AddUObject(this, &UUI_Base::HUD_RenewQuickSlotUI);
	}
}

void UUI_Base::BindInvenComp(UPlayer_InvenComponent* _Comp)
{
	_Comp->OnAcquireItem.AddUObject(this, &UUI_Base::ShowItemAcquireMessage);
	_Comp->OnQSDelay.AddUObject(this, &UUI_Base::SetQuickSlotUIOpacity);
	_Comp->OnQSDelayRate.AddUObject(this, &UUI_Base::SetQuickSlotUIDelay);
	_Comp->OnQSChangeAnim.AddUObject(this, &UUI_Base::PlayQuickSlotChangeAnim);
}

void UUI_Base::BindPlayerWidget(APlayer_Base_Knight* _Character)
{
	_Character->OnSetHUDVisibility.AddUObject(this, &UUI_Base::SetVisibility);
	// UI_Menu_Main
	_Character->OnMenuOpen.AddUObject(this, &UUI_Base::MenuVisibility);	
	// UI_Message_Main, UI_Message_Item
	_Character->OnCloseItemMessageBox.AddUObject(this, &UUI_Base::SetVisibilityItemMessageUI);
	_Character->OnBeginOverlapInteract.AddUObject(this, &UUI_Base::BeginOverlapInteract);
	_Character->OnEndOverlapItem.AddUObject(this, &UUI_Base::EndOverlapItem);
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
		if (m_UI_MessageBox_Item->GetVisibility() == ESlateVisibility::Collapsed)
		{
			m_UI_MessageBox_Main->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUI_Base::ShowItemMessageUI(bool _bShow)
{
	if ( _bShow )
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		m_UI_MessageBox_Item->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUI_Base::ShowItemAcquireMessage(FString _Name, int32 _Stack, UTexture2D* _Img)
{
	GetItemMessageUI()->SetItemMessage(_Name, _Img, _Stack);
	ShowItemMessageUI(true);
	ShowMainMessageUI(true);
}

void UUI_Base::SetVisibilityItemMessageUI()
{
	if ( m_UI_MessageBox_Item->GetVisibility() == ESlateVisibility::HitTestInvisible)
	{
		ShowItemMessageUI(false);
		ShowMainMessageUI(false);
	}
}

void UUI_Base::RenewAmountSoul(int32 _GainedSoul)
{
	m_UI_Soul->RenewAmountOfSoul(_GainedSoul);
}

void UUI_Base::RenewUI_HP(float _CurRatio)
{
	m_UI_PlayerBar->SetPlayerHPRatio(_CurRatio);
}

void UUI_Base::RenewUI_MP(float _CurRatio)
{
	m_UI_PlayerBar->SetPlayerMPRatio(_CurRatio);
}

void UUI_Base::RenewUI_ST(float _CurRatio)
{
	m_UI_PlayerBar->SetPlayerSTRatio(_CurRatio);
}

void UUI_Base::HUD_RenewQuickSlotUI(UItem_InvenData* _CurItem, UItem_InvenData* _NextItem)
{
	m_UI_QuickSlotMain->RenewLowerQuickSlot(_CurItem);
	m_UI_QuickSlotMain->RenewNextLowerQuickSlot(_NextItem);
}

void UUI_Base::SetQuickSlotUIOpacity(bool _IsDelay)
{
	if ( _IsDelay )
	{
		m_UI_QuickSlotMain->SetQuickSlotOpacity(0.5f, false);
	}
	else
	{
		m_UI_QuickSlotMain->SetQuickSlotOpacity(1.f, false);
	}
}

void UUI_Base::SetQuickSlotUIDelay(float _DelayPercnet)
{
	m_UI_QuickSlotMain->SetLowerSlotDelay(_DelayPercnet);
}

void UUI_Base::PlayQuickSlotChangeAnim()
{
	m_UI_QuickSlotMain->PlayQuickSlotAnimation();
}

void UUI_Base::BeginOverlapInteract(FText _Command, FText _Action)
{
	SetMainMessageUI(_Command, _Action);
	ShowMainMessageUI(true);
}

void UUI_Base::EndOverlapItem()
{
	if ( m_UI_MessageBox_Item->GetVisibility() == ESlateVisibility::HitTestInvisible)
	{
		SetMainMessageUI(FText::FromString(L"F"), FText::FromString(L"확인"));
		ShowMainMessageUI(true);
	}
	else
	{
		ShowMainMessageUI(false);
	}
}
