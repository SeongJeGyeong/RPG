// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Boss.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../Monsters/Boss_Base.h"

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

void UUI_Boss::BindBossWidget(ABoss_Base* _Boss)
{
	_Boss->OnDamagedBossHP.AddUObject(this, &UUI_Boss::TakeDamaged);
}

void UUI_Boss::UnBindBossWidget(ABoss_Base* _Boss)
{
	_Boss->OnDamagedBossHP.RemoveAll(this);
}

void UUI_Boss::TakeDamaged(float _Ratio, float _DMG)
{
	SetHPRatio(_Ratio);
	DisplayDMG(_DMG);
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
	m_BossDMGFigure->SetVisibility(ESlateVisibility::HitTestInvisible);
	GetWorld()->GetTimerManager().ClearTimer(BossDmgDisplayTimer);
	GetWorld()->GetTimerManager().SetTimer(BossDmgDisplayTimer, [this]()
	{
		fTakedDMG = 0.f;
		m_BossDMGFigure->SetVisibility(ESlateVisibility::Hidden);
	}, 
	0.1f, false, 3.f);
}