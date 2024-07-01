// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Settings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../System/DataAsset/DA_MenuSound.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "../System/PlayerState_Base.h"


void UUI_Settings::NativeConstruct()
{
	if ( !IsValid(m_Btn_Quit) )
	{
		UE_LOG(LogTemp, Error, TEXT("종료 버튼 위젯 로드 실패"));
	}
	else
	{
		m_Btn_Quit->OnClicked.AddDynamic(this, &UUI_Settings::QuitBtnClicked);
		m_Btn_Quit->OnHovered.AddDynamic(this, &UUI_Settings::QuitBtnHovered);
		m_Btn_Quit->OnUnhovered.AddDynamic(this, &UUI_Settings::QuitBtnUnHovered);
	}

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if (!IsValid(m_Sound))
	{
		UE_LOG(LogTemp, Error, TEXT("메뉴 사운드 로드 실패"));
	}

	OnNativeVisibilityChanged.AddUObject(this, &UUI_Settings::SettingsVisibilityChanged);

	if (IsValid(m_PlayerName))
	{
		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
		m_PlayerName->SetText(FText::FromString(pPlayerState->GetPlayerName()));
	}

	Super::NativeConstruct();
}

void UUI_Settings::SettingsVisibilityChanged(ESlateVisibility _Visibility)
{
	if (_Visibility != ESlateVisibility::Visible)
	{
		return;
	}

	APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (IsValid(m_Playtime))
	{
		uint32 CurrentTime = FMath::Floor(GetWorld()->GetTimeSeconds());
		uint32 ElapsedTime = CurrentTime - pPlayerState->GetPlayTime();

		uint32 sec = pPlayerState->GetPlayTime() + ElapsedTime;
		pPlayerState->SetPlayTime(sec);

		uint32 min = sec / 60;	// 초를 분으로 바꿈
		uint32 hour = min / 60;	// 분을 시로 바꿈
		sec = sec % 60;			// 초를 분으로 바꾸고 남은 초를 저장
		min = min % 60;			// 분을 시로 바꾸고 남은 분을 저장

		FText hourTxt = UKismetTextLibrary::Conv_IntToText(hour, false, true, 2);
		FText minTxt = UKismetTextLibrary::Conv_IntToText(min, false, true, 2);
		FText secTxt = UKismetTextLibrary::Conv_IntToText(sec, false, true, 2);
		TArray<FText> TimeTxtArr = {hourTxt, minTxt, secTxt};

		FText TimeTxt = FText::Join(FText::FromString(TEXT(":")), TimeTxtArr);
		m_Playtime->SetText(TimeTxt);
	}

	if (IsValid(m_Level))
	{
		m_Level->SetText(FText::FromString(FString::Printf(TEXT("%d"), pPlayerState->GetPlayerLevel())));
	}

	if (IsValid(m_MapName))
	{
		m_MapName->SetText(FText::FromString(GetWorld()->GetMapName()));
	}
}

void UUI_Settings::QuitBtnClicked()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_OPEN));
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UUI_Settings::QuitBtnHovered()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
}

void UUI_Settings::QuitBtnUnHovered()
{
}
