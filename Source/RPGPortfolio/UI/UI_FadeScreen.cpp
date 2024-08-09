// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_FadeScreen.h"
#include "Components/Border.h"

void UUI_FadeScreen::NativeConstruct()
{
	if ( !IsValid(m_Screen) )
	{
		UE_LOG(LogTemp, Error, TEXT("페이드 스크린 UI 로드 실패"));
	}
	else
	{
		m_Screen->SetVisibility(ESlateVisibility::Visible);
	}

	Super::NativeConstruct();
}

void UUI_FadeScreen::FadeIn(float _Time)
{
	m_Screen->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 1.f));
	SetVisibility(ESlateVisibility::Visible);
	fFadeAlpha = 1.f;
	fFadeTime = 0.01f / _Time;

	GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this]
		{
			fFadeAlpha = FMath::Clamp(fFadeAlpha - fFadeTime, 0.f, 1.f);
			m_Screen->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, fFadeAlpha));
			if (fFadeAlpha <= 0.f )
			{
				SetVisibility(ESlateVisibility::Hidden);
				GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
				return;
			}
		}
	, 0.01f, true);
}

void UUI_FadeScreen::FadeOut(float _Time)
{
	m_Screen->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
	SetVisibility(ESlateVisibility::Visible);
	fFadeAlpha = 0.f;
	fFadeTime = 0.01f / _Time;

	GetWorld()->GetTimerManager().SetTimer(FadeTimer, [this]
		{
			fFadeAlpha = FMath::Clamp(fFadeAlpha + fFadeTime, 0.f, 1.f);
			m_Screen->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, fFadeAlpha));
			if ( fFadeAlpha >= 1.f )
			{
				GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
				return;
			}
		}
	, 0.01f, true);
}
