// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Menu_Main.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Header/Enum.h"

void UUI_Menu_Main::NativeConstruct()
{
	if (!IsValid(m_Btn_Equip) || !IsValid(m_Btn_Inventory) || !IsValid(m_Btn_Status) || !IsValid(m_Btn_Manual) || !IsValid(m_Btn_Settings))
	{
		UE_LOG(LogTemp, Error, TEXT("Menu Button Load Failed"));
	}
	else
	{
		m_Btn_Equip->OnClicked.AddDynamic(this, &UUI_Menu_Main::EquipBtnClicked);
		m_Btn_Equip->OnHovered.AddDynamic(this, &UUI_Menu_Main::EquipBtnHovered);
		m_Btn_Equip->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::EquipBtnUnHovered);

		m_Btn_Inventory->OnClicked.AddDynamic(this, &UUI_Menu_Main::InventoryBtnClicked);
		m_Btn_Inventory->OnHovered.AddDynamic(this, &UUI_Menu_Main::InventoryBtnHovered);
		m_Btn_Inventory->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::InventoryBtnUnHovered);

		m_Btn_Status->OnClicked.AddDynamic(this, &UUI_Menu_Main::StatusBtnClicked);
		m_Btn_Status->OnHovered.AddDynamic(this, &UUI_Menu_Main::StatusBtnHovered);
		m_Btn_Status->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::StatusBtnUnHovered);

		m_Btn_Manual->OnClicked.AddDynamic(this, &UUI_Menu_Main::ManualBtnClicked);
		m_Btn_Manual->OnHovered.AddDynamic(this, &UUI_Menu_Main::ManualBtnHovered);
		m_Btn_Manual->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::ManualBtnUnHovered);

		m_Btn_Settings->OnClicked.AddDynamic(this, &UUI_Menu_Main::SettingsBtnClicked);
		m_Btn_Settings->OnHovered.AddDynamic(this, &UUI_Menu_Main::SettingsBtnHovered);
		m_Btn_Settings->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::SettingsBtnUnHovered);
	}

	if (!IsValid(m_MenuName))
	{
		UE_LOG(LogTemp, Error, TEXT("Menu Text Load Failed"));
	}

	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( IsValid(GameMode) )
	{
		GameMode->BindMenuUI(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
	}

	Super::NativeConstruct();
}

void UUI_Menu_Main::EquipBtnClicked()
{
	OnEquipUIOpen.Broadcast(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::EquipBtnHovered()
{
	// 문자열 앞에 L을 붙여야 유니코드로 인식함
	m_MenuName->SetText(FText::FromString(L"장비"));
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::EquipBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::InventoryBtnClicked()
{
	OnInventoryUIOpen.Broadcast(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::InventoryBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"인벤토리"));
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::InventoryBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::StatusBtnClicked()
{
	OnStatusUIOpen.Broadcast(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::StatusBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"스테이터스"));
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::StatusBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::ManualBtnClicked()
{
	OnManualUIOpen.Broadcast(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::ManualBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"조작법"));
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::ManualBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::SettingsBtnClicked()
{
	OnSettingsUIOpen.Broadcast(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::SettingsBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"설정"));
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::SettingsBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}
