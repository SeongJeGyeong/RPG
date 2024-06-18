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

	UE_LOG(LogTemp, Warning, TEXT("physicAtk : %f"), PlayerBasePower.PhysicAtk);
	UE_LOG(LogTemp, Warning, TEXT("physicDef : %f"), PlayerBasePower.PhysicDef);
	UE_LOG(LogTemp, Warning, TEXT("MagicAtk : %f"), PlayerBasePower.MagicAtk);
	UE_LOG(LogTemp, Warning, TEXT("MagicDef : %f"), PlayerBasePower.MagicDef);

	m_PhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
	m_MagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicAtk)));
	m_PhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
	m_MagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicDef)));
}

void UUI_PlayerStat::SetVisibilityAlterBasePower(bool _bVisibility)
{
	if(_bVisibility == true)
	{
		m_AltPhysicAtk->SetVisibility(ESlateVisibility::Visible);
		m_AltPhysicDef->SetVisibility(ESlateVisibility::Visible);
		m_AltMagicAtk->SetVisibility(ESlateVisibility::Visible);
		m_AltMagicDef->SetVisibility(ESlateVisibility::Visible);
		m_PhyAtk_Arrow->SetVisibility(ESlateVisibility::Visible);
		m_PhyDef_Arrow->SetVisibility(ESlateVisibility::Visible);
		m_MagAtk_Arrow->SetVisibility(ESlateVisibility::Visible);
		m_MagDef_Arrow->SetVisibility(ESlateVisibility::Visible);
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
	APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	FCharacterBasePower PlayerBasePower = pPlayerState->GetPlayerBasePower();

	// 이미 장착되어 있는 아이템일 경우 장착해제 처리
	if(_bEquiped)
	{
		if ( _InvenData->GetPhysicAtkVal() > 0 )
		{
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.PhysicAtk - _InvenData->GetPhysicAtkVal()))));
		}
		else
		{
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
		}

		if ( _InvenData->GetMagicAtkVal() > 0 )
		{
			m_AltMagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.MagicAtk - _InvenData->GetMagicAtkVal()))));
		}
		else
		{
			m_AltMagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicAtk)));
		}

		if ( _InvenData->GetPhysicDefVal() > 0 )
		{
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.PhysicDef - _InvenData->GetPhysicDefVal()))));
		}
		else
		{
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
		}

		if ( _InvenData->GetMagicDefVal() > 0 )
		{
			m_AltMagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.MagicDef - _InvenData->GetMagicDefVal()))));
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
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.PhysicAtk - pPlayerState->GetEquipmentStatus().Wea_PhyAtk + _InvenData->GetPhysicAtkVal()))));
		}
		else
		{
			m_AltPhysicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicAtk)));
		}

		if ( _InvenData->GetMagicAtkVal() > 0 )
		{
			m_AltMagicAtk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)(PlayerBasePower.MagicAtk - pPlayerState->GetEquipmentStatus().Wea_MagAtk + _InvenData->GetMagicAtkVal()))));
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
			fEquipItemPhyDef = pPlayerState->GetEquipmentStatus().Helm_PhyDef;
			fEquipItemMagDef = pPlayerState->GetEquipmentStatus().Helm_MagDef;
			break;
		case EITEM_TYPE::ARM_CHEST:
			fEquipItemPhyDef = pPlayerState->GetEquipmentStatus().Chest_PhyDef;
			fEquipItemMagDef = pPlayerState->GetEquipmentStatus().Chest_MagDef;
			break;
		case EITEM_TYPE::ARM_GAUNTLET:
			fEquipItemPhyDef = pPlayerState->GetEquipmentStatus().Gaunt_PhyDef;
			fEquipItemMagDef = pPlayerState->GetEquipmentStatus().Gaunt_MagDef;
			break;
		case EITEM_TYPE::ARM_LEGGINGS:
			fEquipItemPhyDef = pPlayerState->GetEquipmentStatus().Leg_PhyDef;
			fEquipItemMagDef = pPlayerState->GetEquipmentStatus().Leg_MagDef;
			break;
		default:
			break;
		}

		if ( _InvenData->GetPhysicDefVal() > 0 )
		{
			
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)( PlayerBasePower.PhysicDef - fEquipItemPhyDef + _InvenData->GetPhysicDefVal() ))));
		}
		else
		{
			m_AltPhysicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.PhysicDef)));
		}

		if ( _InvenData->GetMagicDefVal() > 0 )
		{
			m_AltMagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)( PlayerBasePower.MagicDef - fEquipItemMagDef + _InvenData->GetMagicDefVal() ))));
		}
		else
		{
			m_AltMagicDef->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)PlayerBasePower.MagicDef)));
		}
	}
}
