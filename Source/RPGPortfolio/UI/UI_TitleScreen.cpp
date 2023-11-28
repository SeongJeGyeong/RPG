// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TitleScreen.h"
#include "Components/Button.h"

void UUI_TitleScreen::NativeConstruct()
{
	Super::NativeConstruct();

	m_NewBtn = Cast<UButton>(GetWidgetFromName(FName("NewBtn")));
	m_LoadBtn = Cast<UButton>(GetWidgetFromName(FName("LoadBtn")));
	m_QuitBtn = Cast<UButton>(GetWidgetFromName(FName("QuitBtn")));

	if (!IsValid(m_NewBtn) || !IsValid(m_LoadBtn) || !IsValid(m_QuitBtn))
	{
		UE_LOG(LogTemp, Error, TEXT("Title Screen Button Load Failed"));
	}
	else
	{
		m_NewBtn->OnClicked.AddDynamic(this, &UUI_TitleScreen::NewBtnClicked);
		m_NewBtn->OnHovered.AddDynamic(this, &UUI_TitleScreen::NewBtnHovered);
		m_NewBtn->OnUnhovered.AddDynamic(this, &UUI_TitleScreen::NewBtnUnHovered);

		m_LoadBtn->OnClicked.AddDynamic(this, &UUI_TitleScreen::LoadBtnClicked);
		m_LoadBtn->OnHovered.AddDynamic(this, &UUI_TitleScreen::LoadBtnHovered);
		m_LoadBtn->OnUnhovered.AddDynamic(this, &UUI_TitleScreen::LoadBtnUnHovered);

		m_QuitBtn->OnClicked.AddDynamic(this, &UUI_TitleScreen::QuitBtnClicked);
		m_QuitBtn->OnHovered.AddDynamic(this, &UUI_TitleScreen::QuitBtnHovered);
		m_QuitBtn->OnUnhovered.AddDynamic(this, &UUI_TitleScreen::QuitBtnUnHovered);
	}
}

void UUI_TitleScreen::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_TitleScreen::NewBtnClicked()
{
}

void UUI_TitleScreen::NewBtnHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("NewButton Hovered"));

	UWidgetBlueprintGeneratedClass* pWidgetClass = GetWidgetTreeOwningClass();

	for (int32 i = 0; i < pWidgetClass->Animations.Num(); ++i)
	{
		if (pWidgetClass->Animations[i].GetName() == TEXT("NewBtnHoveredAnim_INST"))
		{
			PlayAnimation(pWidgetClass->Animations[i]);
			SetNumLoopsToPlay(pWidgetClass->Animations[i], 0);
			break;
		}
	}
}

void UUI_TitleScreen::NewBtnUnHovered()
{
}

void UUI_TitleScreen::LoadBtnClicked()
{
}

void UUI_TitleScreen::LoadBtnHovered()
{
	UWidgetBlueprintGeneratedClass* pWidgetClass = GetWidgetTreeOwningClass();

	for (int32 i = 0; i < pWidgetClass->Animations.Num(); ++i)
	{
		if (pWidgetClass->Animations[i].GetName() == TEXT("LoadBtnHoveredAnim_INST"))
		{
			PlayAnimation(pWidgetClass->Animations[i]);
			SetNumLoopsToPlay(pWidgetClass->Animations[i], 0);
			break;
		}
	}
}

void UUI_TitleScreen::LoadBtnUnHovered()
{
}

void UUI_TitleScreen::QuitBtnClicked()
{
}

void UUI_TitleScreen::QuitBtnHovered()
{
	UWidgetBlueprintGeneratedClass* pWidgetClass = GetWidgetTreeOwningClass();

	for (int32 i = 0; i < pWidgetClass->Animations.Num(); ++i)
	{
		if (pWidgetClass->Animations[i].GetName() == TEXT("QuitBtnHoveredAnim_INST"))
		{
			PlayAnimation(pWidgetClass->Animations[i]);
			SetNumLoopsToPlay(pWidgetClass->Animations[i], 0);
			break;
		}
	}
}

void UUI_TitleScreen::QuitBtnUnHovered()
{
}
