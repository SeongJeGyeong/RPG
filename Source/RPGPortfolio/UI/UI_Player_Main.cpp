// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_Main.h"
#include "Components/ProgressBar.h"
#include "../System/PlayerState_Base.h"
#include "Kismet/GameplayStatics.h"

void UUI_Player_Main::NativeConstruct()
{
	Super::NativeConstruct();

	m_HP = Cast<UProgressBar>(GetWidgetFromName(TEXT("PlayerHP")));
	m_MP = Cast<UProgressBar>(GetWidgetFromName(TEXT("PlayerMP")));
	m_Stamina = Cast<UProgressBar>(GetWidgetFromName(TEXT("PlayerStamina")));

	if (!IsValid(m_HP) || !IsValid(m_MP) || !IsValid(m_Stamina))
	{
		UE_LOG(LogTemp, Warning, TEXT("프로그레스바 UI 캐스팅 실패"));
	}
	else
	{
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		FCharacterBasePower BasePower = pPlayerState->GetPlayerBasePower();
	}
}

void UUI_Player_Main::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Player_Main::SetPlayerHPRatio(float _HPRatio)
{
	m_HP->SetPercent(_HPRatio);
}

void UUI_Player_Main::SetPlayerMPRatio(float _MPRatio)
{
	m_MP->SetPercent(_MPRatio);
}
