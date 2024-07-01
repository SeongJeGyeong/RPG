// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Monster.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UUI_Monster::NativeConstruct()
{
	// NativeOnInitialized 함수는 몬스터 이름과 체력을 가져오기 전에 호출되기 때문에 이름과 체력이 ui에 표시되지 않는다.
	// 또한 몬스터는 여러 마리 존재하므로 캐릭터가 존재하는 레벨에 몬스터가 많을수록 몬스터 ui가 동시에 초기화 될 때 부하가 걸릴 수 있다.
	if (!IsValid(m_MonsterName) || !IsValid(m_MonsterHP))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterUI Casting Failed"));
	}
	else
	{
		m_MonsterName->SetText(m_Name);
		m_MonsterHP->SetPercent(m_Ratio);
	}

	if (!IsValid(m_MonDMGFigure))
	{
		UE_LOG(LogTemp, Error, TEXT("DMGFigure Casting Failed"));
	}

	Super::NativeConstruct();
}

void UUI_Monster::NativeDestruct()
{
	Super::NativeDestruct();
	fDisplayTime = 0.f;
	fTakedDMG = 0.f;
}

void UUI_Monster::SetHPRatio(float _Ratio)
{
	// ui 생성 전에 비율 저장
	m_Ratio = _Ratio;

	if (IsValid(m_MonsterHP))
	{
		// ui 생성 후 비율 재설정시 사용
		m_MonsterHP->SetPercent(m_Ratio);
	}
}

void UUI_Monster::SetName(const FString& _Name)
{
	// ui 생성 전에 이름 저장
	m_Name = FText::FromString(_Name);
}

void UUI_Monster::DisplayDMG(const float _DMG)
{
	if (!IsValid(m_MonDMGFigure))
	{
		m_MonDMGFigure = Cast<UTextBlock>(GetWidgetFromName(TEXT("DMGFigure")));
	}
	fTakedDMG += _DMG;
	m_MonDMGFigure->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int)fTakedDMG)));
	m_MonDMGFigure->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().ClearTimer(DmgDisplayTimer);
	GetWorld()->GetTimerManager().SetTimer(DmgDisplayTimer, [this]()
	{
		fTakedDMG = 0.f;
		m_MonDMGFigure->SetVisibility(ESlateVisibility::Hidden);
	},
	0.1f, false, 3.f);
}