// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_StatMgr.h"
#include "Kismet/GameplayStatics.h"

UGISubsystem_StatMgr::UGISubsystem_StatMgr()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterSheet(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_CharacterSheet.DT_CharacterSheet'"));
	if ( CharacterSheet.Succeeded() || CharacterSheet.Object->GetRowStruct()->IsChildOf(FCharacterStatSheet::StaticStruct()) )
	{
		FCharacterStatSheet* pCharacterStat;
		pCharacterStat = CharacterSheet.Object->FindRow<FCharacterStatSheet>(FName("Knight"), TEXT(""));
		PlayerStat = *pCharacterStat;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterSoul(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_PlayerAmountOfSoul.DT_PlayerAmountOfSoul'"));
	if ( CharacterSoul.Succeeded() )
	{
		PlayerBasePower.AmountOfSoul = CharacterSoul.Object->FindRow<FPlayerAmountOfSoul>(FName("Knight"), TEXT(""))->AmountOfSoul;
	}

	PlayerBasePower.MaxHP = PlayerStat.Vigor * 100.f;
	PlayerBasePower.CurHP = PlayerBasePower.MaxHP;
	PlayerBasePower.MaxMP = PlayerStat.Attunement * 10.f;
	PlayerBasePower.CurMP = PlayerBasePower.MaxMP;
	PlayerBasePower.MaxStamina = PlayerStat.Endurance * 10.f;
	PlayerBasePower.CurStamina = PlayerBasePower.MaxStamina;
	PlayerBasePower.PhysicAtk = ( PlayerStat.Strength + PlayerStat.Dexterity ) * 10.f;
	PlayerBasePower.PhysicDef = ( PlayerStat.Strength + PlayerStat.Dexterity ) * 5.f;
	PlayerBasePower.MagicAtk = PlayerStat.Intelligence * 20.f;
	PlayerBasePower.MagicDef = ( PlayerStat.Attunement + PlayerStat.Intelligence ) * 5.f;
}

void UGISubsystem_StatMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGISubsystem_StatMgr::Deinitialize()
{
	OnRenewAmountSoul.Clear();
	OnRenewHP.Clear();
	OnRenewMP.Clear();
	OnRenewST.Clear();

	Super::Deinitialize();
}

void UGISubsystem_StatMgr::SetAtkAndDef()
{
	PlayerBasePower.PhysicAtk = ( ( PlayerStat.Strength + PlayerStat.Dexterity ) * 10.f ) + EquipmentStat.Wea_PhyAtk;
	PlayerBasePower.MagicAtk = ( PlayerStat.Intelligence * 20.f ) + EquipmentStat.Wea_MagAtk;
	PlayerBasePower.PhysicDef = ( ( PlayerStat.Strength + PlayerStat.Dexterity ) * 5.f ) + ( EquipmentStat.Helm_PhyDef + EquipmentStat.Chest_PhyDef + EquipmentStat.Gaunt_PhyDef + EquipmentStat.Leg_PhyDef );
	PlayerBasePower.MagicDef = ( ( PlayerStat.Attunement + PlayerStat.Intelligence ) * 5.f ) + ( EquipmentStat.Helm_MagDef + EquipmentStat.Chest_MagDef + EquipmentStat.Gaunt_MagDef + EquipmentStat.Leg_MagDef );
}

void UGISubsystem_StatMgr::SetEquipFigure(FGameItemInfo* _ItemInfo, bool bEquiped)
{
	switch ( _ItemInfo->Type )
	{
	case EITEM_TYPE::WEAPON:
		EquipmentStat.Wea_PhyAtk = bEquiped ? _ItemInfo->PhysicAtk : 0.f;
		EquipmentStat.Wea_MagAtk = bEquiped ? _ItemInfo->MagicAtk : 0.f;
		break;
	case EITEM_TYPE::ARM_HELM:
		EquipmentStat.Helm_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		EquipmentStat.Helm_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
		break;
	case EITEM_TYPE::ARM_CHEST:
		EquipmentStat.Chest_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		EquipmentStat.Chest_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
		break;
	case EITEM_TYPE::ARM_GAUNTLET:
		EquipmentStat.Gaunt_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		EquipmentStat.Gaunt_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
		break;
	case EITEM_TYPE::ARM_LEGGINGS:
		EquipmentStat.Leg_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		EquipmentStat.Leg_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
		break;
	case EITEM_TYPE::ACCESSORIE:
		break;
	default:
		break;
	}
}

void UGISubsystem_StatMgr::SetPlayerCurrentHP(float _CurHP)
{
	PlayerBasePower.CurHP = _CurHP;
	OnRenewHP.Broadcast(PlayerBasePower.CurHP / PlayerBasePower.MaxHP);
}

void UGISubsystem_StatMgr::SetPlayerCurrentMP(float _CurMP)
{
	PlayerBasePower.CurMP = _CurMP;
	OnRenewMP.Broadcast(PlayerBasePower.CurMP / PlayerBasePower.MaxMP);
}

void UGISubsystem_StatMgr::SetPlayerCurrentStamina(float _CurStamina)
{
	PlayerBasePower.CurStamina = _CurStamina;
	OnRenewST.Broadcast(PlayerBasePower.CurStamina / PlayerBasePower.MaxStamina);
	GetWorld()->GetTimerManager().ClearTimer(RecoveryTimer);
	
	if (PlayerBasePower.CurStamina != PlayerBasePower.MaxStamina)
	{
		GetWorld()->GetTimerManager().SetTimer(RecoveryTimer, this, &UGISubsystem_StatMgr::StaminaRecoveryStart, 0.01f, true, 1.5f);
	}
}

void UGISubsystem_StatMgr::PlayerGainSoul(int32 _Soul)
{
	PlayerBasePower.AmountOfSoul += _Soul;
	OnRenewAmountSoul.Broadcast(_Soul);
}

void UGISubsystem_StatMgr::StaminaRecoveryStart()
{
	if (PlayerBasePower.CurStamina == PlayerBasePower.MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(RecoveryTimer);
		return;
	}

	if ( RecoveryType == ERecoveryType::SLOW )
	{
		PlayerBasePower.CurStamina = FMath::Clamp(PlayerBasePower.CurStamina + 0.25f, 0.f, PlayerBasePower.MaxStamina);
	}
	else
	{
		PlayerBasePower.CurStamina = FMath::Clamp(PlayerBasePower.CurStamina + 0.5f, 0.f, PlayerBasePower.MaxStamina);
	}

	OnRenewST.Broadcast(PlayerBasePower.CurStamina / PlayerBasePower.MaxStamina);
}
