// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Base.h"
#include "../Manager/Equip_Mgr.h"
#include "../UI/UI_PlayerStat.h"

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

void APlayerState_Base::SetPlayerStat(FCharacterStatSheet _PlayerStat)
{
}

void APlayerState_Base::SetPlayerBasePower()
{
	m_PlayerBasePower.PhysicAtk = ((m_PlayerStat.Strength + m_PlayerStat.Dexterity) * 10.f) + Wea_PhyAtk;
	m_PlayerBasePower.MagicAtk = ( m_PlayerStat.Intelligence * 20.f ) + Wea_MagAtk;
	m_PlayerBasePower.PhysicDef = ((m_PlayerStat.Strength + m_PlayerStat.Dexterity) * 5.f) + (Helm_PhyDef + Chest_PhyDef + Gaunt_PhyDef + Leg_PhyDef);
	m_PlayerBasePower.MagicDef = ((m_PlayerStat.Attunement + m_PlayerStat.Intelligence) * 5.f) + (Helm_MagDef + Chest_MagDef + Gaunt_MagDef + Leg_MagDef);
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

void APlayerState_Base::SetEquipFigure(FGameItemInfo* _ItemInfo, bool bEquiped)
{
	switch ( _ItemInfo->Type )
	{
	case EITEM_TYPE::WEAPON:
		if (bEquiped == false)
		{
			Wea_PhyAtk = 0.f;
			Wea_MagAtk = 0.f;
		}
		else
		{
			Wea_PhyAtk = _ItemInfo->PhysicAtk;
			Wea_MagAtk = _ItemInfo->MagicAtk;
		}
		break;
	case EITEM_TYPE::ARM_HELM:
		if (bEquiped == false)
		{
			Helm_PhyDef = 0.f;
			Helm_MagDef = 0.f;
		}
		else
		{
			Helm_PhyDef = _ItemInfo->PhysicDef;
			Helm_MagDef = _ItemInfo->MagicDef;
		}
		break;
	case EITEM_TYPE::ARM_CHEST:
		if (bEquiped == false)
		{
			Chest_PhyDef = 0.f;
			Chest_MagDef = 0.f;
		}
		else
		{
			Chest_PhyDef = _ItemInfo->PhysicDef;
			Chest_MagDef = _ItemInfo->MagicDef;
		}
		break;
	case EITEM_TYPE::ARM_GAUNTLET:
		if (bEquiped == false)
		{
			Gaunt_PhyDef = 0.f;
			Gaunt_MagDef = 0.f;
		}
		else
		{
			Gaunt_PhyDef = _ItemInfo->PhysicDef;
			Gaunt_MagDef = _ItemInfo->MagicDef;
		}
		break;
	case EITEM_TYPE::ARM_LEGGINGS:
		if (bEquiped == false)
		{
			Leg_PhyDef = 0.f;
			Leg_MagDef = 0.f;
		}
		else
		{
			Leg_PhyDef = _ItemInfo->PhysicDef;
			Leg_MagDef = _ItemInfo->MagicDef;
		}
		break;
	case EITEM_TYPE::ACCESSORIE:
		break;
	default:
		break;
	}
}

void APlayerState_Base::SetPlayerCurrentHP(float _CurHP)
{
	m_PlayerBasePower.CurHP = _CurHP;
}

void APlayerState_Base::SetPlayerCurrentMP(float _CurMP)
{
	m_PlayerBasePower.CurMP = _CurMP;
}
