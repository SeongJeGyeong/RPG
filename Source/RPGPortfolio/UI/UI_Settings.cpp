// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Settings.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "../GameInstance_Base.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Manager/GISubsystem_StatMgr.h"

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

	if ( !IsValid(m_Btn_GSettings) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임설정 버튼 위젯 로드 실패"));
	}
	else
	{
		m_Btn_GSettings->OnClicked.AddDynamic(this, &UUI_Settings::GSettingBtnClicked);
		m_Btn_GSettings->OnHovered.AddDynamic(this, &UUI_Settings::GSettingBtnHovered);
		m_Btn_GSettings->OnUnhovered.AddDynamic(this, &UUI_Settings::GSettingBtnUnHovered);
	}

	if ( !IsValid(m_Btn_Apply) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임설정 버튼 위젯 로드 실패"));
	}
	else
	{
		m_Btn_Apply->OnClicked.AddDynamic(this, &UUI_Settings::ApplyBtnClicked);
		m_Btn_Apply->OnHovered.AddDynamic(this, &UUI_Settings::ApplyBtnHovered);
		m_Btn_Apply->OnUnhovered.AddDynamic(this, &UUI_Settings::ApplyBtnUnHovered);
	}

	OnNativeVisibilityChanged.AddUObject(this, &UUI_Settings::SettingsVisibilityChanged);

	if (IsValid(m_PlayerName))
	{
		UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
		m_PlayerName->SetText(FText::FromString(pGameInst->GetSubsystem<UGISubsystem_StatMgr>()->GetPlayerName()));
	}

	GSettingsPannel->SetVisibility(ESlateVisibility::Hidden);

	Super::NativeConstruct();
}

bool UUI_Settings::GetGameSettingPannelVisibility()
{
	return GSettingsPannel->GetVisibility() == ESlateVisibility::Visible;
}

void UUI_Settings::CloseGameSettingPannel()
{
	GSettingsPannel->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Settings::SettingsVisibilityChanged(ESlateVisibility _Visibility)
{
	if (_Visibility != ESlateVisibility::Visible)
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

	if (IsValid(m_MapName))
	{
		FString LevelName = GetWorld()->GetMapName();
		LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		m_MapName->SetText(FText::FromString(LevelName));
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

void UUI_Settings::QuitBtnUnHovered()
{
}

void UUI_Settings::GSettingBtnClicked()
{
	GSettingsPannel->SetVisibility(ESlateVisibility::Visible);

	UGISubsystem_SoundMgr* SoundMgr = GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>();
	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Settings::GSettingBtnHovered()
{
	UGISubsystem_SoundMgr* SoundMgr = GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>();
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Settings::GSettingBtnUnHovered()
{
}

void UUI_Settings::ApplyBtnClicked()
{
	FString sini = GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni;
	UE_LOG(LogTemp, Warning, TEXT("EditorSettings Path : %s"), *GEditorSettingsIni);
	FString IniFileLocation = FPaths::GeneratedConfigDir() + UGameplayStatics::GetPlatformName() + "/" + GGameUserSettingsIni + ".ini";
	UE_LOG(LogTemp, Warning, TEXT("GameUserSettings Path : %s"), *IniFileLocation);
	UE_LOG(LogTemp, Warning, TEXT("apply ini file : %s"), *sini);
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	if (GetWorld()->WorldType == EWorldType::Game)
	{
		pGameInst->ExecuteResoltionCommand();
	}
	pGameInst->ApplyMasterVolume();
	UGameUserSettings::GetGameUserSettings()->SaveConfig();

	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Settings::ApplyBtnHovered()
{
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}

void UUI_Settings::ApplyBtnUnHovered()
{
}
