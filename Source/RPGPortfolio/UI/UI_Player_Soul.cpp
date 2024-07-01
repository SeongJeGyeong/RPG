// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_Soul.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../System/PlayerState_Base.h"
#include "../System/DataAsset/DA_MenuSound.h"

void UUI_Player_Soul::NativeConstruct()
{
	if (!IsValid(m_Soul) || !IsValid(m_GainSoul))
	{
		UE_LOG(LogTemp, Error, TEXT("소지소울 UI 캐스팅 실패"));
	}
	else
	{
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		FCharacterBasePower PlayerBasePower = pPlayerState->GetPlayerBasePower();
		iDisplayedSoul = PlayerBasePower.AmountOfSoul;
		m_Soul->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerBasePower.AmountOfSoul)));
	}

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if ( !IsValid(m_Sound) )
	{
		UE_LOG(LogTemp, Error, TEXT("소울 UI 사운드 로드 실패"));
	}

	Super::NativeConstruct();
}

void UUI_Player_Soul::RenewAmountOfSoul(int32 _GainedSoul)
{
	iGainedSoul = _GainedSoul;
	m_GainSoul->SetText(FText::FromString(FString::Printf(TEXT("+%d"), iGainedSoul)));
	fOpacity = 1.f;
	m_GainSoul->SetOpacity(fOpacity);
	m_GainSoul->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(SoulGainTimer, this, &UUI_Player_Soul::StartSoulGain, 2.f, false);
}

void UUI_Player_Soul::StartSoulGain()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::SOUL_SUCK));

	GetWorld()->GetTimerManager().ClearTimer(SoulGainTimer);
	GetWorld()->GetTimerManager().SetTimer(RenewSoulTimer, this, &UUI_Player_Soul::SoulGain, 0.01f, true);
	GetWorld()->GetTimerManager().SetTimer(FadeOutSoulTimer, this, &UUI_Player_Soul::FadeOutSoul, 0.01f, true);
}

// 획득한 소울 점진적으로 증가하는 것처럼 보이게
void UUI_Player_Soul::SoulGain()
{
	APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	FCharacterBasePower PlayerBasePower = pPlayerState->GetPlayerBasePower();

	if ( iDisplayedSoul < PlayerBasePower.AmountOfSoul )
	{
		iDisplayedSoul = FMath::Clamp(iDisplayedSoul + iGainedSoul * 3 * GetWorld()->GetDeltaSeconds(), iDisplayedSoul, PlayerBasePower.AmountOfSoul);
		m_Soul->SetText(FText::FromString(FString::Printf(TEXT("%d"), iDisplayedSoul)));
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(RenewSoulTimer);
		iDisplayedSoul = PlayerBasePower.AmountOfSoul;
	}
}

// 획득한 소울 표시 페이드아웃
void UUI_Player_Soul::FadeOutSoul()
{
	fOpacity = FMath::Clamp(fOpacity - 0.01f, 0.f, 1.f);
	m_GainSoul->SetOpacity(fOpacity);
	if ( fOpacity <= 0.f )
	{
		m_GainSoul->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(FadeOutSoulTimer);
	}
}
