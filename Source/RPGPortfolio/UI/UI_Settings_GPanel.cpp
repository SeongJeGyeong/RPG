// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Settings_GPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "../GameInstance_Base.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "UI_GameSettingBox.h"

void UUI_Settings_GPanel::NativeConstruct()
{
	SB_ScreenMode->SetSettingText(L"화면 모드");
	SB_Resolution->SetSettingText(L"해상도");
	SB_ViewDistance->SetSettingText(L"뷰 디스턴스");
	SB_AntiAliasing->SetSettingText(L"안티 에일리어싱");
	SB_Shadow->SetSettingText(L"섀도");
	SB_PostProcessing->SetSettingText(L"포스트 프로세싱");
	SB_GlobalIllumination->SetSettingText(L"글로벌 일루미네이션");
	SB_Reflection->SetSettingText(L"리플렉션");
	SB_Texture->SetSettingText(L"텍스쳐");
	SB_Effect->SetSettingText(L"이펙트");
	SB_Foliage->SetSettingText(L"폴리지");
	SB_Shading->SetSettingText(L"셰이딩");
	SB_VSync->SetSettingText(L"VSync");
	SB_Sound->SetSettingText(L"사운드 볼륨");

	if (!IsValid(m_Btn_Apply))
	{
		UE_LOG(LogTemp, Error, TEXT("게임설정 버튼 위젯 로드 실패"));
	}
	else
	{
		m_Btn_Apply->OnClicked.AddDynamic(this, &UUI_Settings_GPanel::ApplyBtnClicked);
		m_Btn_Apply->OnHovered.AddDynamic(this, &UUI_Settings_GPanel::ApplyBtnHovered);
	}

	OnNativeVisibilityChanged.AddUObject(this, &UUI_Settings_GPanel::GameSettingsVisibilityChanged);

	Super::NativeConstruct();
}

void UUI_Settings_GPanel::GameSettingsVisibilityChanged(ESlateVisibility _Visibility)
{
	SB_ScreenMode->InitSettingProperty(_Visibility);
	SB_Resolution->InitSettingProperty(_Visibility);
	SB_ViewDistance->InitSettingProperty(_Visibility);
	SB_AntiAliasing->InitSettingProperty(_Visibility);
	SB_Shadow->InitSettingProperty(_Visibility);
	SB_PostProcessing->InitSettingProperty(_Visibility);
	SB_GlobalIllumination->InitSettingProperty(_Visibility);
	SB_Reflection->InitSettingProperty(_Visibility);
	SB_Texture->InitSettingProperty(_Visibility);
	SB_Effect->InitSettingProperty(_Visibility);
	SB_Foliage->InitSettingProperty(_Visibility);
	SB_Shading->InitSettingProperty(_Visibility);
	SB_VSync->InitSettingProperty(_Visibility);
	SB_Sound->InitSettingProperty(_Visibility);
}

void UUI_Settings_GPanel::ApplyBtnClicked()
{
	//FString sini = GIsEditor ? GEditorSettingsIni : GGameUserSettingsIni;
	//FString IniFileLocation = FPaths::GeneratedConfigDir() + UGameplayStatics::GetPlatformName() + "/" + GGameUserSettingsIni + ".ini";
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	// 윈도우모드에 따라 렌더링 방식이 다르다
	// 전체화면 : GPU 리소스를 직접 제어하는 독점적 모드 사용
	// 전체 창모드 : 시스템 창 관리자를 통해 동작
	// 해상도 -> 윈도우모드 순으로 적용하지 않으면 해상도나 비율을 잘못 인식하거나 제대로 갱신되지 않을 수 있음
	UGameUserSettings::GetGameUserSettings()->SetScreenResolution(pGameInst->GetTempResolution());
	UGameUserSettings::GetGameUserSettings()->ApplyResolutionSettings(false);
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(pGameInst->GetTempWindowMode());
	UGameUserSettings::GetGameUserSettings()->ApplySettings(false);

	// 스탠드얼론으로 실행 시
	if ( GetWorld()->WorldType == EWorldType::Game )
	{
		pGameInst->ExecuteResoltionCommand();
	}
	pGameInst->ApplyMasterVolume();
	UGameUserSettings::GetGameUserSettings()->SaveConfig();

	PlaySound(GETMENUSOUND(EMenuSound::MENU_OPEN));
}

void UUI_Settings_GPanel::ApplyBtnHovered()
{
	PlaySound(GETMENUSOUND(EMenuSound::MENU_SELECT));
}
