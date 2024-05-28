// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Boss.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UUI_Boss::NativeConstruct()
{
	Super::NativeConstruct();
	m_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("BossName")));
	m_DMGFigure = Cast<UTextBlock>(GetWidgetFromName(TEXT("DMGFigure")));
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BossHP")));

	if (!IsValid(m_Name) || !IsValid(m_HPBar) || !IsValid(m_DMGFigure))
	{
		UE_LOG(LogTemp, Error, TEXT("BossUI Casting Failed"));
	}
	else
	{
		m_DMGFigure->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_Boss::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Boss::SetHPRatio(float _Ratio)
{
	m_HPBar->SetPercent(_Ratio);
}

void UUI_Boss::SetName(const FString& _Name)
{
	m_Name->SetText(FText::FromString(_Name));
}

void UUI_Boss::DisplayDMG(const float _DMG)
{
	fTakedDMG += _DMG;
	m_DMGFigure->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)fTakedDMG)));
	m_DMGFigure->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().ClearTimer(BossDmgDisplayTimer);
	GetWorld()->GetTimerManager().SetTimer(BossDmgDisplayTimer, [this]()
	{
		fTakedDMG = 0.f;
		m_DMGFigure->SetVisibility(ESlateVisibility::Hidden);
	}, 
	0.1f, false, 3.f);
}