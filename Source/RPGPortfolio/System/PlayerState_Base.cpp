// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Base.h"
#include "../Manager/Equip_Mgr.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_Player_Main.h"
#include "RPGPortfolio/RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"

APlayerState_Base::APlayerState_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	/*ConstructorHelpers::FObjectFinder<UDataTable> CharacterSheet(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_CharacterSheet.DT_CharacterSheet'"));
	if ( CharacterSheet.Succeeded() || CharacterSheet.Object->GetRowStruct()->IsChildOf(FCharacterStatSheet::StaticStruct()) )
	{
		FCharacterStatSheet* pCharacterStat;
		pCharacterStat = CharacterSheet.Object->FindRow<FCharacterStatSheet>(FName("Knight"), TEXT(""));
		m_PlayerStat = *pCharacterStat;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> CharacterSoul(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_PlayerAmountOfSoul.DT_PlayerAmountOfSoul'"));
	if ( CharacterSoul.Succeeded() )
	{
		m_PlayerBasePower.AmountOfSoul = CharacterSoul.Object->FindRow<FPlayerAmountOfSoul>(FName("Knight"), TEXT(""))->AmountOfSoul;
	}

	m_PlayerBasePower.MaxHP = m_PlayerStat.Vigor * 100.f;
	m_PlayerBasePower.CurHP = m_PlayerBasePower.MaxHP;
	m_PlayerBasePower.MaxMP = m_PlayerStat.Attunement * 10.f;
	m_PlayerBasePower.CurMP = m_PlayerBasePower.MaxMP;
	m_PlayerBasePower.MaxStamina = m_PlayerStat.Endurance * 10.f;
	m_PlayerBasePower.CurStamina = m_PlayerBasePower.MaxStamina;
	m_PlayerBasePower.PhysicAtk = ( ( m_PlayerStat.Strength + m_PlayerStat.Dexterity ) / 2 ) * 20.f;
	m_PlayerBasePower.PhysicDef = ( ( m_PlayerStat.Strength + m_PlayerStat.Dexterity ) / 2 ) * 10.f;
	m_PlayerBasePower.MagicAtk = m_PlayerStat.Intelligence * 20.f;
	m_PlayerBasePower.MagicDef = ( ( m_PlayerStat.Attunement + m_PlayerStat.Intelligence ) / 2 ) * 10.f;*/
}

void APlayerState_Base::BeginPlay()
{
	Super::BeginPlay();

	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( IsValid(GameMode) )
	{
		UUI_Base* pMainUI = GameMode->GetMainHUD();

		m_UI = pMainUI->GetMainUIWidget();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("플레이어 스테이트 게임모드 캐스팅 실패"));
	}

	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	if ( IsValid(pGameInst) )
	{
		m_PlayerStat = pGameInst->GetPlayerStatus();
		m_PlayerBasePower = pGameInst->GetPlayerBasePower();
		m_EquipmentStat = pGameInst->GetEquipmentStatInfo();
		SetPlayerCurrentHP(m_PlayerBasePower.CurHP);
		SetPlayerCurrentMP(m_PlayerBasePower.CurMP);
		SetPlayerCurrentStamina(m_PlayerBasePower.CurStamina);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("게임 인스턴스 로드 실패"));
	}

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &APlayerState_Base::SavePlayerStat);
}

void APlayerState_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 스태미너 회복 대기시간
	if (!bSTRecovery && m_PlayerBasePower.CurStamina != m_PlayerBasePower.MaxStamina)
	{
		fSTRecoveryWait += DeltaTime;
		if (fSTRecoveryWait > 1.f)
		{
			bSTRecovery = true;
			fSTRecoveryWait = 0.f;
		}
	}

	// 스태미너 회복
	if (bSTRecovery)
	{
		if (bSTRecovSlowly)
		{
			m_PlayerBasePower.CurStamina = FMath::Clamp(m_PlayerBasePower.CurStamina + 10.f * DeltaTime, 0.f, m_PlayerBasePower.MaxStamina);
		}
		else
		{
			m_PlayerBasePower.CurStamina = FMath::Clamp(m_PlayerBasePower.CurStamina + 25.f * DeltaTime, 0.f, m_PlayerBasePower.MaxStamina);
		}

		m_UI->SetPlayerSTRatio(m_PlayerBasePower.CurStamina / m_PlayerBasePower.MaxStamina);
		if (m_PlayerBasePower.CurStamina == m_PlayerBasePower.MaxStamina)
		{
			bSTRecovery = false;
			fSTRecoveryWait = 0.f;
		}
	}

}

void APlayerState_Base::SavePlayerStat(const FString& MapName)
{
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	if (IsValid(pGameInst))
	{
		pGameInst->SetPlayerStatus(m_PlayerStat);
		pGameInst->SetPlayerBasePower(m_PlayerBasePower);
		pGameInst->SetEquipmentStatInfo(m_EquipmentStat);
		UE_LOG(LogTemp, Warning, TEXT("스탯 세이브"));
	}
}

void APlayerState_Base::SetAtkAndDef()
{
	m_PlayerBasePower.PhysicAtk = ((m_PlayerStat.Strength + m_PlayerStat.Dexterity) * 10.f) + m_EquipmentStat.Wea_PhyAtk;
	m_PlayerBasePower.MagicAtk = ( m_PlayerStat.Intelligence * 20.f ) + m_EquipmentStat.Wea_MagAtk;
	m_PlayerBasePower.PhysicDef = ((m_PlayerStat.Strength + m_PlayerStat.Dexterity) * 5.f) + ( m_EquipmentStat.Helm_PhyDef + m_EquipmentStat.Chest_PhyDef + m_EquipmentStat.Gaunt_PhyDef + m_EquipmentStat.Leg_PhyDef);
	m_PlayerBasePower.MagicDef = ((m_PlayerStat.Attunement + m_PlayerStat.Intelligence) * 5.f) + ( m_EquipmentStat.Helm_MagDef + m_EquipmentStat.Chest_MagDef + m_EquipmentStat.Gaunt_MagDef + m_EquipmentStat.Leg_MagDef);
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
		m_EquipmentStat.Wea_PhyAtk = bEquiped ? _ItemInfo->PhysicAtk : 0.f;
		m_EquipmentStat.Wea_MagAtk = bEquiped ? _ItemInfo->MagicAtk : 0.f;
		break;
	case EITEM_TYPE::ARM_HELM:
		m_EquipmentStat.Helm_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		m_EquipmentStat.Helm_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
		break;
	case EITEM_TYPE::ARM_CHEST:
		m_EquipmentStat.Chest_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		m_EquipmentStat.Chest_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
		break;
	case EITEM_TYPE::ARM_GAUNTLET:
		m_EquipmentStat.Gaunt_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		m_EquipmentStat.Gaunt_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
		break;
	case EITEM_TYPE::ARM_LEGGINGS:
		m_EquipmentStat.Leg_PhyDef = bEquiped ? _ItemInfo->PhysicDef : 0.f;
		m_EquipmentStat.Leg_MagDef = bEquiped ? _ItemInfo->MagicDef : 0.f;
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
	m_UI->SetPlayerHPRatio(m_PlayerBasePower.CurHP / m_PlayerBasePower.MaxHP);
}

void APlayerState_Base::SetPlayerCurrentMP(float _CurMP)
{
	m_PlayerBasePower.CurMP = _CurMP;
	m_UI->SetPlayerMPRatio(m_PlayerBasePower.CurMP / m_PlayerBasePower.MaxMP);
}

void APlayerState_Base::SetPlayerCurrentStamina(float _CurStamina)
{
	bSTRecovery = false;
	m_PlayerBasePower.CurStamina = _CurStamina;
	m_UI->SetPlayerSTRatio(m_PlayerBasePower.CurStamina / m_PlayerBasePower.MaxStamina);
	fSTRecoveryWait = 0.f;
}

void APlayerState_Base::PlayerGainSoul(int32 _Soul)
{
	m_PlayerBasePower.AmountOfSoul += _Soul;
}