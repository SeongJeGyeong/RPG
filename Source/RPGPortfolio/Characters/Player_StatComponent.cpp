// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_StatComponent.h"
#include "../Manager/GISubsystem_StatMgr.h"

UPlayer_StatComponent::UPlayer_StatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayer_StatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	m_StatMgr = GetOwner()->GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
}


// Called every frame
void UPlayer_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UPlayer_StatComponent::IsHPZero()
{
	return  m_StatMgr->GetPlayerBasePower().CurHP <= 0.f;
}

bool UPlayer_StatComponent::IsStaminaZero()
{
	return m_StatMgr->GetPlayerBasePower().CurStamina <= 0.f;
}

bool UPlayer_StatComponent::IsEnoughStamina(float _Consumption)
{
	return m_StatMgr->GetPlayerBasePower().CurStamina >= _Consumption;
}

bool UPlayer_StatComponent::IsEnoughMP(float _Consumption)
{
	return m_StatMgr->GetPlayerBasePower().CurMP >= _Consumption;
}

float UPlayer_StatComponent::CalculApplyDamage(EATTACK_TYPE _AtkType, EPlayerMontage _AtkMontage)
{
	float fDamage = 0.f;
	switch ( _AtkType )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		fDamage = m_StatMgr->GetPlayerBasePower().PhysicAtk;
		if ( _AtkMontage == EPlayerMontage::HEAVYATTACK )
		{
			fDamage = fDamage * 1.5f;
		}
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		fDamage = m_StatMgr->GetPlayerBasePower().MagicAtk;
		break;
	default:
		break;
	}

	return fDamage;
}

float UPlayer_StatComponent::CalculTakeDamage(EATTACK_TYPE _AtkType, float _Damage)
{
	float fPlayerDef = 0.f;
	switch ( _AtkType )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		fPlayerDef = m_StatMgr->GetPlayerBasePower().PhysicDef;
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		fPlayerDef = m_StatMgr->GetPlayerBasePower().MagicDef;
		break;
	default:
		break;
	}

	return FMath::Clamp(_Damage - fPlayerDef, 0.f, _Damage);
}

void UPlayer_StatComponent::DecreasePlayerHP(EATTACK_TYPE _AtkType, float _Damage)
{
	float FinalDamage = CalculTakeDamage(_AtkType, _Damage);
	float CurHP = m_StatMgr->GetPlayerBasePower().CurHP;
	CurHP = FMath::Clamp(CurHP - FinalDamage, 0.f, m_StatMgr->GetPlayerBasePower().MaxHP);
	m_StatMgr->SetPlayerCurrentHP(CurHP);
}

void UPlayer_StatComponent::DecreasePlayerMP(float _Consumption)
{
	float CurMP = m_StatMgr->GetPlayerBasePower().CurMP;
	CurMP = FMath::Clamp(CurMP - _Consumption, 0.f, m_StatMgr->GetPlayerBasePower().MaxMP);
	m_StatMgr->SetPlayerCurrentMP(CurMP);
}

void UPlayer_StatComponent::DecreasePlayerStamina(float _Consumption)
{
	float CurStamina = m_StatMgr->GetPlayerBasePower().CurStamina;
	CurStamina = FMath::Clamp(CurStamina - _Consumption, 0.f, m_StatMgr->GetPlayerBasePower().MaxStamina);
	m_StatMgr->SetPlayerCurrentStamina(CurStamina);
}

void UPlayer_StatComponent::RestorePlayerHP(float _Recovery)
{
	m_StatMgr->SetPlayerCurrentHP(FMath::Clamp(m_StatMgr->GetPlayerBasePower().CurHP + _Recovery, 0.f, m_StatMgr->GetPlayerBasePower().MaxHP));
}

void UPlayer_StatComponent::RestorePlayerMP(float _Recovery)
{
	m_StatMgr->SetPlayerCurrentMP(FMath::Clamp(m_StatMgr->GetPlayerBasePower().CurMP + _Recovery, 0.f, m_StatMgr->GetPlayerBasePower().MaxMP));
}

void UPlayer_StatComponent::GainSoul(int32 _Soul)
{
	m_StatMgr->PlayerGainSoul(_Soul);
}

void UPlayer_StatComponent::HoldGuard(bool _IsGuard)
{
	m_StatMgr->SetbSTRecovSlowly(_IsGuard);
}
