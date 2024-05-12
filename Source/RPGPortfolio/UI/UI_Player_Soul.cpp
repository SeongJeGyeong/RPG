// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_Soul.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../System/PlayerState_Base.h"
#include "../System/DataAsset/DA_MenuSound.h"

void UUI_Player_Soul::NativeConstruct()
{
	Super::NativeConstruct();

	m_AmountOfSoul = Cast<UTextBlock>(GetWidgetFromName(TEXT("AmountOfSoul")));
	m_GainedSoul = Cast<UTextBlock>(GetWidgetFromName(TEXT("GainedSoul")));

	if (!IsValid(m_AmountOfSoul) || !IsValid(m_GainedSoul))
	{
		UE_LOG(LogTemp, Error, TEXT("소지소울 UI 캐스팅 실패"));
	}
	else
	{
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		FCharacterBasePower PlayerBasePower = pPlayerState->GetPlayerBasePower();
		iDisplayedSoul = PlayerBasePower.AmountOfSoul;
		m_AmountOfSoul->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerBasePower.AmountOfSoul)));
	}

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if ( !IsValid(m_Sound) )
	{
		UE_LOG(LogTemp, Error, TEXT("소울 UI 사운드 로드 실패"));
	}
}

void UUI_Player_Soul::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);

	// 획득한 소울 점진적으로 증가하는 것처럼 보이게
	if (bSoulGained)
	{
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		FCharacterBasePower PlayerBasePower = pPlayerState->GetPlayerBasePower();

		if (iDisplayedSoul < PlayerBasePower.AmountOfSoul)
		{
			iDisplayedSoul = FMath::Clamp(iDisplayedSoul + iGainedSoul * 3.f * _DeltaTime, iDisplayedSoul, PlayerBasePower.AmountOfSoul);
			m_AmountOfSoul->SetText(FText::FromString(FString::Printf(TEXT("%d"), iDisplayedSoul)));
		}
		else
		{
			bSoulGained = false;
			iDisplayedSoul = PlayerBasePower.AmountOfSoul;
		}
	}

	// 획득한 소울 표시 페이드아웃
	if (bFadeOutGainedSoul)
	{
		fOpacity = FMath::Clamp(fOpacity - _DeltaTime, 0.f, 1.f);
		m_GainedSoul->SetOpacity(fOpacity);
		if (fOpacity <= 0.f)
		{
			m_GainedSoul->SetVisibility(ESlateVisibility::Hidden);
			bFadeOutGainedSoul = false;
		}
	}
}

void UUI_Player_Soul::RenewAmountOfSoul(int32 _GainedSoul)
{
	iGainedSoul = _GainedSoul;
	m_GainedSoul->SetText(FText::FromString(FString::Printf(TEXT("+%d"), iGainedSoul)));
	fOpacity = 1.f;
	m_GainedSoul->SetOpacity(fOpacity);
	m_GainedSoul->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(SoulGainTimer, this, &UUI_Player_Soul::StartSoulGain, 2.f, false);
}

void UUI_Player_Soul::StartSoulGain()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::SOUL_SUCK));
	bFadeOutGainedSoul = true;
	bSoulGained = true;
}
