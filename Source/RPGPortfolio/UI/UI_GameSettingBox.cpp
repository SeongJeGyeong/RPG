// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameSettingBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "../System/DataAsset/DA_MenuSound.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"

void UUI_GameSettingBox::NativeConstruct()
{
	switch (Setting_Property)
	{
	case ESETTING_PROPERTY::SCREEN_MODE:
		Txt_SettingProperty->SetText(FText::FromString(L"화면 모드"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode());
		break;
	case ESETTING_PROPERTY::RESOLUTION:
		Txt_SettingProperty->SetText(FText::FromString(L"해상도"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetScreenResolution());
		break;
	case ESETTING_PROPERTY::VIEW_DISTANCE:
		Txt_SettingProperty->SetText(FText::FromString(L"뷰 디스턴스"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetViewDistanceQuality());
		break;
	case ESETTING_PROPERTY::ANTI_ALIASING:
		Txt_SettingProperty->SetText(FText::FromString(L"안티 에일리어싱"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetAntiAliasingQuality());
		break;
	case ESETTING_PROPERTY::POST_PROCESSING:
		Txt_SettingProperty->SetText(FText::FromString(L"포스트 프로세싱"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetPostProcessingQuality());
		break;
	case ESETTING_PROPERTY::SHADOW:
		Txt_SettingProperty->SetText(FText::FromString(L"섀도"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetShadowQuality());
		break;
	case ESETTING_PROPERTY::GLOBAL_ILLUMINATION:
		Txt_SettingProperty->SetText(FText::FromString(L"글로벌 일루미네이션"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetGlobalIlluminationQuality());
		break;
	case ESETTING_PROPERTY::REFLECTION:
		Txt_SettingProperty->SetText(FText::FromString(L"리플렉션"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetReflectionQuality());
		break;
	case ESETTING_PROPERTY::TEXTURE:
		Txt_SettingProperty->SetText(FText::FromString(L"텍스쳐"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetTextureQuality());
		break;
	case ESETTING_PROPERTY::EFFECT:
		Txt_SettingProperty->SetText(FText::FromString(L"이펙트"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetVisualEffectQuality());
		break;
	case ESETTING_PROPERTY::FOLIAGE:
		Txt_SettingProperty->SetText(FText::FromString(L"폴리지"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetFoliageQuality());
		break;
	case ESETTING_PROPERTY::SHADING:
		Txt_SettingProperty->SetText(FText::FromString(L"셰이딩"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->GetShadingQuality());
		break;
	case ESETTING_PROPERTY::VSYNC:
		Txt_SettingProperty->SetText(FText::FromString(L"VSync"));
		SetUserSettingValText(UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled());
		break;
	case ESETTING_PROPERTY::SOUND:
		{
			Txt_SettingProperty->SetText(FText::FromString(L"사운드 볼륨"));
			UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
			SetUserSettingValText(pGameInst->GetMasterVolume());
			break;
		}
	}

	Btn_Down->OnClicked.AddDynamic(this, &UUI_GameSettingBox::DownBtnClicked);
	Btn_Down->OnHovered.AddDynamic(this, &UUI_GameSettingBox::DownBtnHovered);
	Btn_Up->OnClicked.AddDynamic(this, &UUI_GameSettingBox::UpBtnClicked);
	Btn_Up->OnHovered.AddDynamic(this, &UUI_GameSettingBox::UpBtnHovered);

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if ( !IsValid(m_Sound) )
	{
		UE_LOG(LogTemp, Error, TEXT("메뉴 사운드 로드 실패"));
	}

	Super::NativeConstruct();
}

void UUI_GameSettingBox::OnVisibilityChanged(ESlateVisibility _Visibility)
{

}

void UUI_GameSettingBox::SetUserSettingValText(int32 _Val)
{
	FString PropertyVal;
	switch ( _Val )
	{
	case 0:
		PropertyVal = L"낮음";
		break;
	case 1:
		PropertyVal = L"중간";
		break;
	case 2:
		PropertyVal = L"높음";
		break;
	case 3:
		PropertyVal = L"에픽";
		break;
	case 4:
		PropertyVal = L"시네마틱";
		break;
	}
	Txt_SettingLevel->SetText(FText::FromString(PropertyVal));
}

void UUI_GameSettingBox::SetUserSettingValText(float _Val)
{
	int32 SoundVal = FMath::Clamp((int32)(_Val * 10), 0, 10);
	Txt_SettingLevel->SetText(FText::FromString(FString::Printf(TEXT("%d"), SoundVal)));
}

void UUI_GameSettingBox::SetUserSettingValText(bool _Val)
{
	FString PropertyVal = _Val ? L"On" : L"OFF";
	Txt_SettingLevel->SetText(FText::FromString(PropertyVal));
}

void UUI_GameSettingBox::SetUserSettingValText(FIntPoint _Val)
{
	Txt_SettingLevel->SetText(FText::FromString(FString::Printf(TEXT("%d x %d"), _Val.X, _Val.Y)));
}

void UUI_GameSettingBox::SetUserSettingValText(EWindowMode::Type _Val)
{
	FString PropertyVal;
	switch ( _Val )
	{
	case EWindowMode::Fullscreen:
		PropertyVal = L"전체화면";
		break;
	case EWindowMode::Windowed:
		PropertyVal = L"창 모드";
		break;
	case EWindowMode::WindowedFullscreen:
		PropertyVal = L"전체 창 모드";
		break;
	}
	Txt_SettingLevel->SetText(FText::FromString(PropertyVal));
}

EWindowMode::Type UUI_GameSettingBox::ChangeScreenMode(EWindowMode::Type _Type, bool _UpDown)
{
	EWindowMode::Type Mode;
	switch ( _Type )
	{
	case EWindowMode::Fullscreen:
		Mode = _UpDown ? EWindowMode::Windowed : EWindowMode::WindowedFullscreen;
		break;
	case EWindowMode::WindowedFullscreen:
		Mode = _UpDown ? EWindowMode::Fullscreen : EWindowMode::Windowed;
		break;
	case EWindowMode::Windowed:
		Mode = _UpDown ? EWindowMode::WindowedFullscreen : EWindowMode::Fullscreen;
		break;
	default:
		break;
	}

	return Mode;
}

FIntPoint UUI_GameSettingBox::ChangeScreenResolution(FIntPoint _Res, bool _UpDown)
{
	int32 ResX;
	int32 ResY;
	switch ( _Res.X )
	{
	case 1280:
		ResX = _UpDown ? 1600 : 1280;
		ResY = _UpDown ? 900  : 720;
		break;
	case 1600:
		ResX = _UpDown ? 1920 : 1280;
		ResY = _UpDown ? 1080 : 720;
		break;
	case 1920:
		ResX = _UpDown ? 2560 : 1600;
		ResY = _UpDown ? 1440 : 900;
		break;
	case 2560:
		ResX = _UpDown ? 2560 : 1920;
		ResY = _UpDown ? 1440 : 1080;
		break;
	}

	if (GetWorld()->WorldType == EWorldType::Game)
	{
		UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
		pGameInst->SetTempResolution(FString::Printf(TEXT("%dx%d"), ResX, ResY));
	}

	return FIntPoint(ResX, ResY);
}

void UUI_GameSettingBox::DownBtnClicked()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_CHANGE));

	switch ( Setting_Property )
	{
	case ESETTING_PROPERTY::SCREEN_MODE:
		{
			EWindowMode::Type ScreenMode = ChangeScreenMode(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode(), false);
			SetUserSettingValText(ScreenMode);
			UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(ScreenMode);
			break;
		}
	case ESETTING_PROPERTY::RESOLUTION:
		{
			FIntPoint Resolution = ChangeScreenResolution(UGameUserSettings::GetGameUserSettings()->GetScreenResolution(), false);
			SetUserSettingValText(Resolution);
			UGameUserSettings::GetGameUserSettings()->SetScreenResolution(Resolution);
			break;
		}
	case ESETTING_PROPERTY::VIEW_DISTANCE:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetViewDistanceQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetViewDistanceQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::ANTI_ALIASING:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetAntiAliasingQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::POST_PROCESSING:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetPostProcessingQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetPostProcessingQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::SHADOW:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetShadowQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetShadowQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::GLOBAL_ILLUMINATION:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetGlobalIlluminationQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetGlobalIlluminationQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::REFLECTION:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetReflectionQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetReflectionQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::TEXTURE:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetTextureQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetTextureQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::EFFECT:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetVisualEffectQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetVisualEffectQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::FOLIAGE:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetFoliageQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetFoliageQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::SHADING:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetShadingQuality() - 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetShadingQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::VSYNC:
		{
			bool iVal = UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled() ? false : true;
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetVSyncEnabled(iVal);
			break;
		}
	case ESETTING_PROPERTY::SOUND:
		{
			UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
			float iVal = FMath::Clamp(pGameInst->GetMasterVolume() - 0.1f, 0, 1);
			SetUserSettingValText(iVal);
			pGameInst->SetTempVolume(iVal);
			break;
		}
	}
}

void UUI_GameSettingBox::DownBtnHovered()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_CHANGE));
}

void UUI_GameSettingBox::UpBtnClicked()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_CHANGE));

	switch ( Setting_Property )
	{
	case ESETTING_PROPERTY::SCREEN_MODE:
		{
			EWindowMode::Type ScreenMode = ChangeScreenMode(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode(), true);
			SetUserSettingValText(ScreenMode);
			UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(ScreenMode);
			break;
		}
	case ESETTING_PROPERTY::RESOLUTION:
		{
			FIntPoint Resolution = ChangeScreenResolution(UGameUserSettings::GetGameUserSettings()->GetScreenResolution(), true);
			SetUserSettingValText(Resolution);
			UGameUserSettings::GetGameUserSettings()->SetScreenResolution(Resolution);
			break;
		}
	case ESETTING_PROPERTY::VIEW_DISTANCE:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetViewDistanceQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetViewDistanceQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::ANTI_ALIASING:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetAntiAliasingQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetAntiAliasingQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::POST_PROCESSING:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetPostProcessingQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetPostProcessingQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::SHADOW:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetShadowQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetShadowQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::GLOBAL_ILLUMINATION:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetGlobalIlluminationQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetGlobalIlluminationQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::REFLECTION:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetReflectionQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetReflectionQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::TEXTURE:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetTextureQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetTextureQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::EFFECT:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetVisualEffectQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetVisualEffectQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::FOLIAGE:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetFoliageQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetFoliageQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::SHADING:
		{
			int32 iVal = FMath::Clamp(UGameUserSettings::GetGameUserSettings()->GetShadingQuality() + 1, 0, 4);
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetShadingQuality(iVal);
			break;
		}
	case ESETTING_PROPERTY::VSYNC:
		{
			bool iVal = UGameUserSettings::GetGameUserSettings()->IsVSyncEnabled() ? false : true;
			SetUserSettingValText(iVal);
			UGameUserSettings::GetGameUserSettings()->SetVSyncEnabled(iVal);
			break;
		}
	case ESETTING_PROPERTY::SOUND:
		{
			UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
			float iVal = FMath::Clamp(pGameInst->GetMasterVolume() + 0.1f, 0, 1);
			SetUserSettingValText(iVal);
			pGameInst->SetTempVolume(iVal);
			break;
		}
	}
}

void UUI_GameSettingBox::UpBtnHovered()
{
	PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_CHANGE));
}