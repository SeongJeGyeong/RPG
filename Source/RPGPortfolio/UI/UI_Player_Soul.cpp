// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_Soul.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "../System/PlayerState_Base.h"

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

}

void UUI_Player_Soul::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);

	if (bSoulGained)
	{
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		FCharacterBasePower PlayerBasePower = pPlayerState->GetPlayerBasePower();

		if (iDisplayedSoul < PlayerBasePower.AmountOfSoul)
		{
			iDisplayedSoul = FMath::Clamp(iDisplayedSoul + iGainedSoul * 5.f * _DeltaTime, iDisplayedSoul, PlayerBasePower.AmountOfSoul);
			UE_LOG(LogTemp, Display, TEXT("soul : %d"), iDisplayedSoul);
			m_AmountOfSoul->SetText(FText::FromString(FString::Printf(TEXT("%d"), iDisplayedSoul)));
		}
		else
		{
			bSoulGained = false;
			iDisplayedSoul = PlayerBasePower.AmountOfSoul;
		}
	}

	if (bDisplayGainedSoul)
	{
		fDisplayTime += _DeltaTime;

		if (fDisplayTime >= 3.f)
		{
			fOpacity = FMath::Clamp(fOpacity - _DeltaTime, 0.f, 1.f);
			m_GainedSoul->SetOpacity(fOpacity);
			if (fOpacity <= 0.f)
			{
				m_GainedSoul->SetVisibility(ESlateVisibility::Hidden);
				bDisplayGainedSoul = false;
				fDisplayTime = 0.f;
				fOpacity = 1.f;
			}
		}
	}


}

void UUI_Player_Soul::RenewAmountOfSoul(int32 _GainedSoul)
{
	iGainedSoul = _GainedSoul;
	m_GainedSoul->SetText(FText::FromString(FString::Printf(TEXT("+%d"), iGainedSoul)));
	m_GainedSoul->SetOpacity(1.f);
	fDisplayTime = 0.f;
	fOpacity = 1.f;
	m_GainedSoul->SetVisibility(ESlateVisibility::Visible);
	bSoulGained = true;
	bDisplayGainedSoul = true;
}
