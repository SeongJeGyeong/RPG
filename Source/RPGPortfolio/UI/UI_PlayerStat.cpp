// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerStat.h"
#include "Components/TextBlock.h"
#include "../System/PlayerState_Base.h"
#include "../Header/Struct.h"

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

	if (!IsValid(m_Level) || !IsValid(m_Vigor) || !IsValid(m_Attunement) || !IsValid(m_Endurance) ||
		!IsValid(m_Strength) || !IsValid(m_Dexterity) || !IsValid(m_Intelligence) || !IsValid(m_MaxHP) ||
		!IsValid(m_MaxMP) || !IsValid(m_MaxStamina) || !IsValid(m_CurHP) || !IsValid(m_CurMP) || !IsValid(m_CurStamina))
	{
		UE_LOG(LogTemp, Error, TEXT("캐릭터 스탯창 캐스팅 실패"));
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

}
