// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipMain.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "UI_EquipItem.h"
#include "UI_EquipItemList.h"
#include "../System/PlayerState_Base.h"
#include "Kismet/GameplayStatics.h"

void UUI_EquipMain::NativeConstruct()
{
	Super::NativeConstruct();

	m_SelItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Sel_ItemName")));
	m_SelItemSlot = Cast<UTextBlock>(GetWidgetFromName(TEXT("Sel_ItemSlot")));
	m_Tooltip = Cast<UUI_ItemTooltip>(GetWidgetFromName(TEXT("ItemTooltipUI")));
	m_Stat = Cast<UUI_PlayerStat>(GetWidgetFromName(TEXT("PlayerStatUI")));
	m_ItemList = Cast<UUI_EquipItemList>(GetWidgetFromName(TEXT("ItemListUI")));
	
	if ( !IsValid(m_SelItemName) || !IsValid(m_SelItemSlot) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 텍스트 UI 캐스팅 실패"));
	}

	if ( !IsValid(m_Tooltip) || !IsValid(m_Stat) )
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 서브 UI 캐스팅 실패"));
	}
	else
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		if ( !IsValid(pPlayerState) )
		{
			UE_LOG(LogTemp, Error, TEXT("플레이어스테이트 획득 실패"));
		}
		else
		{
			m_Stat->SetPlayerStatUI(Cast<APlayerState_Base>(pPlayerState));
		}
	}

	if ( !IsValid(m_ItemList) )
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 리스트 UI 캐스팅 실패"));
	}
	else
	{
		m_ItemList->SetVisibility(ESlateVisibility::Hidden);
		OnNativeVisibilityChanged.AddUObject(this, &UUI_EquipMain::ListVisibilityChanged);
	}

}

void UUI_EquipMain::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_EquipMain::ListVisibilityChanged(ESlateVisibility _Visibility)
{
	if (_Visibility == ESlateVisibility::Hidden)
	{
		m_ItemList->SetVisibility(ESlateVisibility::Hidden);
	}
}