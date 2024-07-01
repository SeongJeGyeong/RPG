// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Menu_Main.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Manager/Inventory_Mgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI_StatusMain.h"
#include "UI_EquipMain.h"
#include "UI_Manual.h"
#include "UI_Settings.h"
#include "../System/DataAsset/DA_MenuSound.h"

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

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if ( !IsValid(m_Sound) )
	{
		UE_LOG(LogTemp, Error, TEXT("메뉴 사운드 로드 실패"));
	}

	Super::NativeConstruct();
}

void UUI_Menu_Main::EquipBtnClicked()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}

	UUI_EquipMain* EquipUI = GameMode->GetEquipUI();
	EquipUI->SetVisibility(ESlateVisibility::Visible);
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::EquipBtnHovered()
{
	// 문자열 앞에 L을 붙여야 유니코드로 인식함
	m_MenuName->SetText(FText::FromString(L"장비"));
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::EquipBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::InventoryBtnClicked()
{
	UInventory_Mgr::GetInst(GetWorld())->ShowInventoryUI();
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::InventoryBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"인벤토리"));
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::InventoryBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::StatusBtnClicked()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}

	UUI_StatusMain* StatusUI = GameMode->GetStatusUI();
	StatusUI->SetVisibility(ESlateVisibility::Visible);
	StatusUI->RenewStatusUI();
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::StatusBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"스테이터스"));
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::StatusBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::ManualBtnClicked()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}

	UUI_Manual* ManualUI = GameMode->GetManualUI();
	ManualUI->SetVisibility(ESlateVisibility::Visible);

	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::ManualBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"조작법"));
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::ManualBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::SettingsBtnClicked()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}

	UUI_Settings* SettingsUI = GameMode->GetSettingsUI();
	SettingsUI->SetVisibility(ESlateVisibility::Visible);

	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_OPEN));
}

void UUI_Menu_Main::SettingsBtnHovered()
{
	m_MenuName->SetText(FText::FromString(L"설정"));
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
}

void UUI_Menu_Main::SettingsBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}
