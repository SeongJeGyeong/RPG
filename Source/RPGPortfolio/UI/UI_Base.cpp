// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Base.h"
#include "UI_Player_Main.h"
#include "UI_Player_QuickSlot.h"
#include "UI_Player_Soul.h"
#include "UI_Menu_Main.h"

void UUI_Base::NativeConstruct()
{
	Super::NativeConstruct();

	m_MainUI = Cast<UUI_Player_Main>(GetWidgetFromName(FName("UI_Progress")));
	m_QuickSlotUI = Cast<UUI_Player_QuickSlot>(GetWidgetFromName(FName("UI_ItemSlotMain")));
	m_SoulUI = Cast<UUI_Player_Soul>(GetWidgetFromName(FName("UI_MenuMain")));
	m_MenuUI = Cast<UUI_Menu_Main>(GetWidgetFromName(FName("UI_MenuMain")));

	if (!IsValid(m_MainUI))
	{
		UE_LOG(LogTemp, Error, TEXT("Player Main UI Casting Failed"));
	}
	if (!IsValid(m_QuickSlotUI))
	{
		UE_LOG(LogTemp, Error, TEXT("QuickSlot UI Casting Failed"));
	}
	if (!IsValid(m_SoulUI))
	{
		UE_LOG(LogTemp, Error, TEXT("Amount Of Soul UI Casting Failed"));
	}

	if (!IsValid(m_MenuUI))
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenu UI Casting Failed"));
	}
	else
	{
		m_MenuUI->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUI_Base::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}
