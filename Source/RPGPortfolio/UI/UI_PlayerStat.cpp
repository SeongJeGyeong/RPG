// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerStat.h"
#include "Components/TextBlock.h"
#include "../System/PlayerState_Base.h"
#include "../Header/Struct.h"
#include "../Item/Item_InvenData.h"
#include "Kismet/GameplayStatics.h"

void UUI_PlayerStat::NativeConstruct()
{
	Super::NativeConstruct();

	m_Level = Cast<UTextBlock>(GetWidgetFromName(TEXT("Level")));
	m_Vigor = Cast<UTextBlock>(GetWidgetFromName(TEXT("Vigor")));
	m_Attunement = Cast<UTextBlock>(GetWidgetFromName(TEXT("Attunement")));
	m_Endurance = Cast<UTextBlock>(GetWidgetFromName(TEXT("Endurance")));
	m_Strength = Cast<UTextBlock>(GetWidgetFromName(TEXT("Strength")));
	m_Dexterity = Cast<UTextBlock>(GetWidgetFromName(TEXT("Dexterity")));
	m_Intelligence = Cast<UTextBlock>(GetWidgetFromName(TEXT("Intelligence")));

	m_MaxHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaximumHP")));
	m_MaxMP = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaximumMP")));
	m_MaxStamina = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaximumST")));
	m_CurHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentHP")));
	m_CurMP = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentMP")));
	m_CurStamina = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentST")));

	m_PhysicAtk = Cast<UTextBlock>(GetWidgetFromName(TEXT("PhysicAtk")));
	m_PhysicDef = Cast<UTextBlock>(GetWidgetFromName(TEXT("PhysicDef")));
	m_MagicAtk = Cast<UTextBlock>(GetWidgetFromName(TEXT("MagicAtk")));
	m_MagicDef = Cast<UTextBlock>(GetWidgetFromName(TEXT("MagicDef")));
	m_AltPhysicAtk = Cast<UTextBlock>(GetWidgetFromName(TEXT("Alt_PhysicAtk")));
	m_AltPhysicDef = Cast<UTextBlock>(GetWidgetFromName(TEXT("Alt_PhysicDef")));
	m_AltMagicAtk = Cast<UTextBlock>(GetWidgetFromName(TEXT("Alt_MagicAtk")));
	m_AltMagicDef = Cast<UTextBlock>(GetWidgetFromName(TEXT("Alt_MagicDef")));
	m_PhyAtk_Arrow = Cast<UTextBlock>(GetWidgetFromName(TEXT("Arrow1")));
	m_PhyDef_Arrow = Cast<UTextBlock>(GetWidgetFromName(TEXT("Arrow3")));
	m_MagAtk_Arrow = Cast<UTextBlock>(GetWidgetFromName(TEXT("Arrow2")));
	m_MagDef_Arrow = Cast<UTextBlock>(GetWidgetFromName(TEXT("Arrow4")));


	if (!IsValid(m_Level) || !IsValid(m_Vigor) || !IsValid(m_Attunement) || !IsValid(m_Endurance) ||
		!IsValid(m_Strength) || !IsValid(m_Dexterity) || !IsValid(m_Intelligence) || !IsValid(m_MaxHP) ||
		!IsValid(m_MaxMP) || !IsValid(m_MaxStamina) || !IsValid(m_CurHP) || !IsValid(m_CurMP) || !IsValid(m_CurStamina) ||
		!IsValid(m_PhysicAtk) || !IsValid(m_PhysicDef) || !IsValid(m_MagicAtk) || !IsValid(m_MagicDef) ||
		!IsValid(m_AltPhysicAtk) || !IsValid(m_AltPhysicDef) || !IsValid(m_AltMagicAtk) || !IsValid(m_AltMagicDef) ||
		!IsValid(m_PhyAtk_Arrow) || !IsValid(m_PhyDef_Arrow) || !IsValid(m_MagAtk_Arrow) || !IsValid(m_MagDef_Arrow) 
		)
	{
		UE_LOG(LogTemp, Error, TEXT("캐릭터 스탯창 캐스팅 실패"));
	}
	else
	{
		m_AltPhysicAtk->SetVisibility(ESlateVisibility::Hidden);
		m_AltPhysicDef->SetVisibility(ESlateVisibility::Hidden);
		m_AltMagicAtk->SetVisibility(ESlateVisibility::Hidden);
		m_AltMagicDef->SetVisibility(ESlateVisibility::Hidden);
		m_PhyAtk_Arrow->SetVisibility(ESlateVisibility::Hidden);
		m_PhyDef_Arrow->SetVisibility(ESlateVisibility::Hidden);
		m_MagAtk_Arrow->SetVisibility(ESlateVisibility::Hidden);
		m_MagDef_Arrow->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUI_PlayerStat::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_PlayerStat::SetPlayerStatUI(APlayerState_Base* _PlayerState)
{
	FCharacterStatSheet PlayerStat = _PlayerState->GetPlayerStatus();
	
	m_Level->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Level)));
	m_Vigor->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Vigor)));
	m_Attunement->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Attunement)));
	m_Endurance->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Endurance)));
	m_Strength->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Strength)));
	m_Dexterity->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Dexterity)));
	m_Intelligence->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Intelligence)));

	FCharacterBasePower PlayerBasePower = _PlayerState->GetPlayerBasePower();

	m_MaxHP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MaxHP)));
	m_MaxMP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MaxMP)));
	m_MaxStamina->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MaxStamina)));
	m_CurHP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.CurHP)));
	m_CurMP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.CurMP)));
	m_CurStamina->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.CurStamina)));
	m_PhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
	m_PhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
	m_MagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicAtk)));
	m_MagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicDef)));

}

void UUI_PlayerStat::RenewBasePower()
{
	APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	FCharacterBasePower PlayerBasePower = pPlayerState->GetPlayerBasePower();

	m_PhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
	m_MagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicAtk)));
	m_PhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
	m_MagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicDef)));
}
