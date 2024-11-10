// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_StatusMain.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/GISubsystem_StatMgr.h"
#include "../Manager/GISubsystem_EquipMgr.h"

void UUI_StatusMain::NativeConstruct()
{
	Super::NativeConstruct();

	m_Level = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level")));
	m_Souls = Cast<UTextBlock>(GetWidgetFromName(TEXT("Souls")));
	m_Required_Souls = Cast<UTextBlock>(GetWidgetFromName(TEXT("Required_Souls")));
	m_Vigor = Cast<UTextBlock>(GetWidgetFromName(TEXT("Vigor")));
	m_Attunement = Cast<UTextBlock>(GetWidgetFromName(TEXT("Attunement")));
	m_Endurance = Cast<UTextBlock>(GetWidgetFromName(TEXT("Endurance")));
	m_Strength = Cast<UTextBlock>(GetWidgetFromName(TEXT("Strength")));
	m_Dexterity = Cast<UTextBlock>(GetWidgetFromName(TEXT("Dexterity")));
	m_Intelligence = Cast<UTextBlock>(GetWidgetFromName(TEXT("Intelligence")));
	m_CurrentHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHP")));
	m_MaximumHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaximumHP")));
	m_CurrentMP = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentMP")));
	m_MaximumMP = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaximumMP")));
	m_CurrentST = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentST")));
	m_MaximumST = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaximumST")));
	m_PhysicAtk_Weak = Cast<UTextBlock>(GetWidgetFromName(TEXT("PhysicAtk_Weak")));
	m_PhysicAtk_Strong = Cast<UTextBlock>(GetWidgetFromName(TEXT("PhysicAtk_Strong")));
	m_MagicAtk = Cast<UTextBlock>(GetWidgetFromName(TEXT("MagicAtk")));
	m_PhysicDef = Cast<UTextBlock>(GetWidgetFromName(TEXT("PhysicDef")));
	m_MagicDef = Cast<UTextBlock>(GetWidgetFromName(TEXT("MagicDef")));

	RenewStatusUI();
}

void UUI_StatusMain::BindEquipMgr()
{
	UGISubsystem_EquipMgr* EquipManager = GetGameInstance()->GetSubsystem<UGISubsystem_EquipMgr>();
	if (EquipManager)
	{
		EquipManager->OnRenewStatus.AddUObject(this, &UUI_StatusMain::RenewStatusUI);
	}
}

void UUI_StatusMain::RenewStatusUI()
{
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();

	m_Level->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerLevel())));
	m_Souls->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerBasePower().AmountOfSoul)));
	//m_Required_Souls;
	m_Vigor->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerStatus().Vigor)));
	m_Attunement->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerStatus().Attunement)));
	m_Endurance->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerStatus().Endurance)));
	m_Strength->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerStatus().Strength)));
	m_Dexterity->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerStatus().Dexterity)));
	m_Intelligence->SetText(FText::FromString(FString::Printf(TEXT("%d"), StatMgr->GetPlayerStatus().Intelligence)));
	m_CurrentHP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().CurHP)));
	m_MaximumHP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().MaxHP)));
	m_CurrentMP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().CurMP)));
	m_MaximumMP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().MaxMP)));
	m_CurrentST->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().CurStamina)));
	m_MaximumST->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().MaxStamina)));
	m_PhysicAtk_Weak->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().PhysicAtk)));
	m_PhysicAtk_Strong->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)( StatMgr->GetPlayerBasePower().PhysicAtk * 1.5f ))));
	m_MagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().MagicAtk)));
	m_PhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().PhysicDef)));
	m_MagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)StatMgr->GetPlayerBasePower().MagicDef)));
}
