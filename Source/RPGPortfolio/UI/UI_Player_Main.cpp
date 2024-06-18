// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_Main.h"
#include "Components/ProgressBar.h"

void UUI_Player_Main::NativeConstruct()
{
	if (!IsValid(m_PlayerHP) || !IsValid(m_PlayerMP) || !IsValid(m_PlayerStamina))
	{
		UE_LOG(LogTemp, Warning, TEXT("프로그레스바 UI 캐스팅 실패"));
	}

	Super::NativeConstruct();
}

void UUI_Player_Main::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Player_Main::SetPlayerHPRatio(float _HPRatio)
{
	m_PlayerHP->SetPercent(_HPRatio);
}

void UUI_Player_Main::SetPlayerMPRatio(float _MPRatio)
{
	m_PlayerMP->SetPercent(_MPRatio);
}

void UUI_Player_Main::SetPlayerSTRatio(float _STRatio)
{
	m_PlayerStamina->SetPercent(_STRatio);
}
