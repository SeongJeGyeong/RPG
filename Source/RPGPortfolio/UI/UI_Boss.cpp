// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Boss.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UUI_Boss::NativeConstruct()
{
	if (!IsValid(m_BossDMGFigure))
	{
		UE_LOG(LogTemp, Error, TEXT("보스 데미지 표기 UI 로드 실패"));
	}
	else
	{
		m_BossDMGFigure->SetVisibility(ESlateVisibility::Hidden);
	}

	Super::NativeConstruct();
}

void UUI_Boss::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Boss::SetHPRatio(float _Ratio)
{
	m_BossHP->SetPercent(_Ratio);
}

void UUI_Boss::SetName(const FString& _Name)
{
	m_BossName->SetText(FText::FromString(_Name));
}

void UUI_Boss::DisplayDMG(const float _DMG)
{
	fTakedDMG += _DMG;
	m_BossDMGFigure->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)fTakedDMG)));
	m_BossDMGFigure->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().ClearTimer(BossDmgDisplayTimer);
	GetWorld()->GetTimerManager().SetTimer(BossDmgDisplayTimer, [this]()
	{
		fTakedDMG = 0.f;
		m_BossDMGFigure->SetVisibility(ESlateVisibility::Hidden);
	}, 
	0.1f, false, 3.f);
}