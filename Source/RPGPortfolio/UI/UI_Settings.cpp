// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Settings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "../GameInstance_Base.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Manager/GISubsystem_StatMgr.h"
#include "UI_Settings_GPanel.h"

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
	}

	if ( !IsValid(m_Btn_GSettings) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임설정 버튼 위젯 로드 실패"));
	}
	else
	{
		m_Btn_GSettings->OnClicked.AddDynamic(this, &UUI_Settings::GSettingBtnClicked);
		m_Btn_GSettings->OnHovered.AddDynamic(this, &UUI_Settings::GSettingBtnHovered);
	}

	OnNativeVisibilityChanged.AddUObject(this, &UUI_Settings::SettingsVisibilityChanged);
	if (IsValid(m_PlayerName))
	{
		UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
		m_PlayerName->SetText(FText::FromString(pGameInst->GetSubsystem<UGISubsystem_StatMgr>()->GetPlayerName()));
	}

	m_GSettingsPannel->SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeConstruct();
}

bool UUI_Settings::GetGameSettingPannelVisibility()
{
	return m_GSettingsPannel->GetVisibility() == ESlateVisibility::SelfHitTestInvisible;
}

void UUI_Settings::CloseGameSettingPannel()
{
	m_GSettingsPannel->SetVisibility(ESlateVisibility::Collapsed);
}

void UUI_Settings::SettingsVisibilityChanged(ESlateVisibility _Visibility)
{
	if (_Visibility == ESlateVisibility::Hidden || _Visibility == ESlateVisibility::Collapsed)
	{
		return;
	}

	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	if (IsValid(m_Playtime))
	{
		uint32 CurrentTime = FMath::Floor(GetWorld()->GetTimeSeconds());
		uint32 ElapsedTime = CurrentTime - Cast<UGameInstance_Base>(GetGameInstance())->GetPlayTime();

		uint32 sec = Cast<UGameInstance_Base>(GetGameInstance())->GetPlayTime() + ElapsedTime;
		Cast<UGameInstance_Base>(GetGameInstance())->SetPlayTime(sec);

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
		m_Level->SetText(FText::FromString(FString::Printf(TEXT("%d"), pGameInst->GetSubsystem<UGISubsystem_StatMgr>()->GetPlayerLevel())));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("레벨 가져오기 실패"));
	}

	if (IsValid(m_MapName))
	{
		FString LevelName = GetWorld()->GetMapName();
		LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		m_MapName->SetText(FText::FromString(LevelName));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("맵이름 가져오기 실패"));
	}
}

void UUI_Settings::QuitBtnClicked()
{
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UUI_Settings::QuitBtnHovered()
{
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Settings::GSettingBtnClicked()
{
	m_GSettingsPannel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UGISubsystem_SoundMgr* SoundMgr = GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>();
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Settings::GSettingBtnHovered()
{
	UGISubsystem_SoundMgr* SoundMgr = GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>();
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}