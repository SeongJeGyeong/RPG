﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Menu_Main.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UUI_Menu_Main::NativeConstruct()
{
	Super::NativeConstruct();

	m_Equip = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Equip")));
	m_Inventory = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Inventory")));
	m_Status = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Status")));
	m_Tutorial = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Tutorial")));
	m_Settings = Cast<UButton>(GetWidgetFromName(TEXT("Btn_Settings")));
	m_MenuName = Cast<UTextBlock>(GetWidgetFromName(TEXT("MenuName")));

	if (!IsValid(m_Equip) || !IsValid(m_Inventory) || !IsValid(m_Status) || !IsValid(m_Tutorial) || !IsValid(m_Settings))
	{
		UE_LOG(LogTemp, Error, TEXT("Menu Button Load Failed"));
	}
	else
	{
		m_Equip->OnClicked.AddDynamic(this, &UUI_Menu_Main::EquipBtnClicked);
		m_Equip->OnHovered.AddDynamic(this, &UUI_Menu_Main::EquipBtnHovered);
		m_Equip->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::EquipBtnUnHovered);

		m_Inventory->OnClicked.AddDynamic(this, &UUI_Menu_Main::InventoryBtnClicked);
		m_Inventory->OnHovered.AddDynamic(this, &UUI_Menu_Main::InventoryBtnHovered);
		m_Inventory->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::InventoryBtnUnHovered);

		m_Status->OnClicked.AddDynamic(this, &UUI_Menu_Main::StatusBtnClicked);
		m_Status->OnHovered.AddDynamic(this, &UUI_Menu_Main::StatusBtnHovered);
		m_Status->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::StatusBtnUnHovered);

		m_Tutorial->OnClicked.AddDynamic(this, &UUI_Menu_Main::TutorialBtnClicked);
		m_Tutorial->OnHovered.AddDynamic(this, &UUI_Menu_Main::TutorialBtnHovered);
		m_Tutorial->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::TutorialBtnUnHovered);

		m_Settings->OnClicked.AddDynamic(this, &UUI_Menu_Main::SettingsBtnClicked);
		m_Settings->OnHovered.AddDynamic(this, &UUI_Menu_Main::SettingsBtnHovered);
		m_Settings->OnUnhovered.AddDynamic(this, &UUI_Menu_Main::SettingsBtnUnHovered);
	}

	if (!IsValid(m_MenuName))
	{
		UE_LOG(LogTemp, Error, TEXT("Menu Text Load Failed"));
	}
}

void UUI_Menu_Main::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Menu_Main::EquipBtnClicked()
{
}

void UUI_Menu_Main::EquipBtnHovered()
{
	FText text = FText::FromString("장비");
	FString string = text.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *string);
	m_MenuName->SetText(FText::FromString("Equip"));
}

void UUI_Menu_Main::EquipBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::InventoryBtnClicked()
{
}

void UUI_Menu_Main::InventoryBtnHovered()
{
	m_MenuName->SetText(FText::FromString("Inventory"));
}

void UUI_Menu_Main::InventoryBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::StatusBtnClicked()
{
}

void UUI_Menu_Main::StatusBtnHovered()
{
	m_MenuName->SetText(FText::FromString("Status"));
}

void UUI_Menu_Main::StatusBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::TutorialBtnClicked()
{
}

void UUI_Menu_Main::TutorialBtnHovered()
{
	m_MenuName->SetText(FText::FromString("Tutorial"));
}

void UUI_Menu_Main::TutorialBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}

void UUI_Menu_Main::SettingsBtnClicked()
{
}

void UUI_Menu_Main::SettingsBtnHovered()
{
	m_MenuName->SetText(FText::FromString("Settings"));
}

void UUI_Menu_Main::SettingsBtnUnHovered()
{
	m_MenuName->SetText(FText::GetEmpty());
}
