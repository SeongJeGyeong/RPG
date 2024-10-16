// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerStat.h"
#include "Components/TextBlock.h"
#include "../Header/Struct.h"
#include "../Item/Item_InvenData.h"
#include "Kismet/GameplayStatics.h"
#include "../Manager/GISubsystem_StatMgr.h"

void UUI_PlayerStat::NativeConstruct()
{
	Super::NativeConstruct();

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

void UUI_PlayerStat::SetPlayerStatUI()
{
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	FCharacterStatSheet PlayerStat = StatMgr->GetPlayerStatus();
	
	m_Level->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Level)));
	m_Vigor->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Vigor)));
	m_Attunement->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Attunement)));
	m_Endurance->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Endurance)));
	m_Strength->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Strength)));
	m_Dexterity->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Dexterity)));
	m_Intelligence->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerStat.Intelligence)));

	FCharacterBasePower PlayerBasePower = StatMgr->GetPlayerBasePower();

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
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	FCharacterBasePower PlayerBasePower = StatMgr->GetPlayerBasePower();

	m_PhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
	m_MagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicAtk)));
	m_PhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
	m_MagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicDef)));
}

void UUI_PlayerStat::SetVisibilityAlterBasePower(bool _bVisibility)
{
	if(_bVisibility == true)
	{
		m_AltPhysicAtk->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_AltPhysicDef->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_AltMagicAtk->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_AltMagicDef->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_PhyAtk_Arrow->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_PhyDef_Arrow->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_MagAtk_Arrow->SetVisibility(ESlateVisibility::HitTestInvisible);
		m_MagDef_Arrow->SetVisibility(ESlateVisibility::HitTestInvisible);
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

void UUI_PlayerStat::AlterRenewBasePower(UItem_InvenData* _InvenData, bool _bEquiped)
{
	UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
	FCharacterBasePower PlayerBasePower = StatMgr->GetPlayerBasePower();

	// 이미 장착되어 있는 아이템일 경우 장착해제 처리
	if(_bEquiped)
	{
		if ( _InvenData->GetPhysicAtkVal() > 0 )
		{
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.PhysicAtk - _InvenData->GetPhysicAtkVal()))));
			m_AltPhysicAtk->SetColorAndOpacity(FLinearColor::FLinearColor(0.f, 0.f, 1.f, 1.f));
		}
		else
		{
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
		}

		if ( _InvenData->GetMagicAtkVal() > 0 )
		{
			m_AltMagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.MagicAtk - _InvenData->GetMagicAtkVal()))));
			m_AltMagicAtk->SetColorAndOpacity(FLinearColor::FLinearColor(0.f, 0.f, 1.f, 1.f));
		}
		else
		{
			m_AltMagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicAtk)));
		}

		if ( _InvenData->GetPhysicDefVal() > 0 )
		{
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.PhysicDef - _InvenData->GetPhysicDefVal()))));
			m_AltPhysicDef->SetColorAndOpacity(FLinearColor::FLinearColor(0.f, 0.f, 1.f, 1.f));
		}
		else
		{
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
		}

		if ( _InvenData->GetMagicDefVal() > 0 )
		{
			m_AltMagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.MagicDef - _InvenData->GetMagicDefVal()))));
			m_AltMagicDef->SetColorAndOpacity(FLinearColor::FLinearColor(0.f, 0.f, 1.f, 1.f));
		}
		else
		{
			m_AltMagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicDef)));
		}
	}
	// 장착중인 아이템이 없거나 다른 아이템을 장착중인 경우
	else
	{
		// 현재 장착중인 무기의 공격을 빼고 장착할 무기의 공격력을 더한 값을 표시한다.
		if ( _InvenData->GetPhysicAtkVal() > 0 )
		{
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.PhysicAtk - StatMgr->GetEquipmentStatus().Wea_PhyAtk + _InvenData->GetPhysicAtkVal()))));
			float exPower = PlayerBasePower.PhysicAtk + StatMgr->GetEquipmentStatus().Wea_PhyAtk;
			float newPower = PlayerBasePower.PhysicAtk + _InvenData->GetPhysicAtkVal();
			m_AltPhysicAtk->SetColorAndOpacity(SetRenewPowerTxtsColor(exPower, newPower));
		}
		else
		{
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
		}

		if ( _InvenData->GetMagicAtkVal() > 0 )
		{
			m_AltMagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.MagicAtk - StatMgr->GetEquipmentStatus().Wea_MagAtk + _InvenData->GetMagicAtkVal()))));
			float exPower = PlayerBasePower.MagicAtk + StatMgr->GetEquipmentStatus().Wea_MagAtk;
			float newPower = PlayerBasePower.MagicAtk + _InvenData->GetMagicAtkVal();
			m_AltPhysicAtk->SetColorAndOpacity(SetRenewPowerTxtsColor(exPower, newPower));
		}
		else
		{
			m_AltMagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicAtk)));
		}

		// 현재 장착되어있는 아이템의 방어력을 빼고 장착할 아이템의 방어력을 더한 값을 표시한다.
		float fEquipItemPhyDef = 0.f;
		float fEquipItemMagDef = 0.f;

		switch (_InvenData->GetItemType())
		{
		case EITEM_TYPE::ARM_HELM:
			fEquipItemPhyDef = StatMgr->GetEquipmentStatus().Helm_PhyDef;
			fEquipItemMagDef = StatMgr->GetEquipmentStatus().Helm_MagDef;
			break;
		case EITEM_TYPE::ARM_CHEST:
			fEquipItemPhyDef = StatMgr->GetEquipmentStatus().Chest_PhyDef;
			fEquipItemMagDef = StatMgr->GetEquipmentStatus().Chest_MagDef;
			break;
		case EITEM_TYPE::ARM_GAUNTLET:
			fEquipItemPhyDef = StatMgr->GetEquipmentStatus().Gaunt_PhyDef;
			fEquipItemMagDef = StatMgr->GetEquipmentStatus().Gaunt_MagDef;
			break;
		case EITEM_TYPE::ARM_LEGGINGS:
			fEquipItemPhyDef = StatMgr->GetEquipmentStatus().Leg_PhyDef;
			fEquipItemMagDef = StatMgr->GetEquipmentStatus().Leg_MagDef;
			break;
		default:
			break;
		}

		if ( _InvenData->GetPhysicDefVal() > 0 )
		{
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)( PlayerBasePower.PhysicDef - fEquipItemPhyDef + _InvenData->GetPhysicDefVal() ))));
			float exPower = PlayerBasePower.PhysicDef + fEquipItemPhyDef;
			float newPower = PlayerBasePower.PhysicDef + _InvenData->GetPhysicDefVal();
			m_AltPhysicDef->SetColorAndOpacity(SetRenewPowerTxtsColor(exPower, newPower));
		}
		else
		{
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
		}

		if ( _InvenData->GetMagicDefVal() > 0 )
		{
			m_AltMagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)( PlayerBasePower.MagicDef - fEquipItemMagDef + _InvenData->GetMagicDefVal() ))));
			float exPower = PlayerBasePower.MagicDef + fEquipItemMagDef;
			float newPower = PlayerBasePower.MagicDef + _InvenData->GetMagicDefVal();
			m_AltMagicDef->SetColorAndOpacity(SetRenewPowerTxtsColor(exPower, newPower));
		}
		else
		{
			m_AltMagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicDef)));
		}
	}
}

FLinearColor UUI_PlayerStat::SetRenewPowerTxtsColor(float _ExPower, float _NewPower)
{
	FLinearColor Color;
	if ( _ExPower > _NewPower )
	{
		Color = FLinearColor::FLinearColor(0.1f, 0.1f, 1.f, 1.f);
	}
	else if ( _ExPower < _NewPower )
	{
		Color = FLinearColor::FLinearColor(1.f, 0.f, 0.f, 1.f);
	}
	else
	{
		Color = FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f);
	}

	return Color;
}
