// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Monster.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UUI_Monster::NativeConstruct()
{
	Super::NativeConstruct();

	m_Name = Cast<UTextBlock>(GetWidgetFromName(TEXT("MonsterName")));
	m_DMGFigure = Cast<UTextBlock>(GetWidgetFromName(TEXT("DMGFigure")));
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MonsterHP")));

	if (!IsValid(m_Name) || !IsValid(m_HPBar))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterUI Casting Failed"));
	}
	else
	{
		m_Name->SetText(m_MonsterName);
		m_HPBar->SetPercent(m_Ratio);
	}

	if (!IsValid(m_DMGFigure))
	{
		UE_LOG(LogTemp, Error, TEXT("DMGFigure Casting Failed"));
	}
	else
	{
		m_DMGFigure->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_Monster::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Monster::SetHPRatio(float _Ratio)
{
	// ui 생성 전에 비율 저장
	m_Ratio = _Ratio;

	if (IsValid(m_HPBar))
	{
		// ui 생성 후 비율 재설정시 사용
		m_HPBar->SetPercent(m_Ratio);
	}
}

void UUI_Monster::SetName(const FString& _Name)
{
	// ui 생성 전에 이름 저장
	m_MonsterName = FText::FromString(_Name);

	//if (IsValid(m_Name))
	//{
	//	
	//	m_Name->SetText(FText::FromString(_Name));
	//}
}

void UUI_Monster::DisplayDMG(const int32 _DMG)
{
}