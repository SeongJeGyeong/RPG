// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Base.h"

APlayerState_Base::APlayerState_Base()
{
	ConstructorHelpers::FObjectFinder<UDataTable> CharacterSheet(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_CharacterSheet.DT_CharacterSheet'"));
	if (CharacterSheet.Succeeded() || CharacterSheet.Object->GetRowStruct()->IsChildOf(FCharacterStatSheet::StaticStruct()))
	{
		FCharacterStatSheet* pCharacterStat;
		pCharacterStat = CharacterSheet.Object->FindRow<FCharacterStatSheet>(FName("Knight"), TEXT("Knight"));
		m_PlayerStat = *pCharacterStat;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> CharacterSoul(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_PlayerAmountOfSoul.DT_PlayerAmountOfSoul'"));
	if (CharacterSoul.Succeeded())
	{
		m_PlayerBasePower.AmountOfSoul = CharacterSoul.Object->FindRow<FPlayerAmountOfSoul>(FName("Knight"), TEXT("Knight"))->AmountOfSoul;
	}

	m_PlayerBasePower.MaxHP = m_PlayerStat.Vigor * 100.f;
	m_PlayerBasePower.CurHP = m_PlayerBasePower.MaxHP;
	m_PlayerBasePower.MaxMP = m_PlayerStat.Attunement * 10.f;
	m_PlayerBasePower.CurMP = m_PlayerBasePower.MaxMP;
	m_PlayerBasePower.MaxStamina = m_PlayerStat.Endurance * 10.f;
	m_PlayerBasePower.CurStamina = m_PlayerBasePower.MaxStamina;
	m_PlayerBasePower.PhysicAtk = ( m_PlayerStat.Strength + m_PlayerStat.Dexterity ) * 10.f;
	m_PlayerBasePower.PhysicDef = ( m_PlayerStat.Strength + m_PlayerStat.Dexterity ) * 5.f;
	m_PlayerBasePower.MagicAtk = m_PlayerStat.Intelligence * 20.f;
	m_PlayerBasePower.MagicDef = ( m_PlayerStat.Attunement + m_PlayerStat.Intelligence ) * 5.f;
}

void APlayerState_Base::InitPlayerStat(FCharacterStatSheet _PlayerStat)
{
}

void APlayerState_Base::SetPlayerStat(FCharacterStatSheet _PlayerStat)
{
}

void APlayerState_Base::InitPlayerData(FCharacterBasePower _PlayerBasePower)
{
	m_PlayerBasePower.MaxHP = m_PlayerStat.Vigor * 100.f;
	m_PlayerBasePower.CurHP = m_PlayerBasePower.MaxHP;
	m_PlayerBasePower.MaxMP = m_PlayerStat.Attunement * 10.f;
	m_PlayerBasePower.CurMP = m_PlayerBasePower.MaxMP;
	m_PlayerBasePower.MaxStamina = m_PlayerStat.Endurance * 10.f;
	m_PlayerBasePower.CurStamina = m_PlayerBasePower.MaxStamina;
	m_PlayerBasePower.PhysicAtk = (m_PlayerStat.Strength + m_PlayerStat.Dexterity) * 10.f;
	m_PlayerBasePower.PhysicDef = (m_PlayerStat.Strength + m_PlayerStat.Dexterity) * 5.f;
	m_PlayerBasePower.MagicAtk = m_PlayerStat.Intelligence * 20.f;
	m_PlayerBasePower.MagicDef = (m_PlayerStat.Attunement + m_PlayerStat.Intelligence) * 5.f;
}
