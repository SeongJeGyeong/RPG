// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_EquipMain.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "../System/PlayerState_Base.h"
#include "Kismet/GameplayStatics.h"

void UUI_EquipMain::NativeConstruct()
{
	Super::NativeConstruct();

	m_Tooltip = Cast<UUI_ItemTooltip>(GetWidgetFromName(TEXT("ItemTooltipUI")));
	m_Stat = Cast<UUI_PlayerStat>(GetWidgetFromName(TEXT("PlayerStatUI")));

	if (!IsValid(m_Tooltip) || !IsValid(m_Stat))
	{
		UE_LOG(LogTemp, Error, TEXT("장비창 UI 캐스팅 실패"));
	}
	else
	{
		m_Tooltip->SetVisibility(ESlateVisibility::Hidden);
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		if (!IsValid(pPlayerState))
		{
			UE_LOG(LogTemp, Error, TEXT("플레이어스테이트 획득 실패"));
		}
		else
		{
			m_Stat->SetPlayerStatUI(Cast<APlayerState_Base>(pPlayerState));
		}
	}
}

void UUI_EquipMain::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}
