// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameSettingBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"
#include "../Manager/GISubsystem_SoundMgr.h"

void UUI_GameSettingBox::NativeConstruct()
{
	Btn_Down->OnClicked.AddDynamic(this, &UUI_GameSettingBox::DownBtnClicked);
	Btn_Down->OnHovered.AddDynamic(this, &UUI_GameSettingBox::DownBtnHovered);
	Btn_Up->OnClicked.AddDynamic(this, &UUI_GameSettingBox::UpBtnClicked);
	Btn_Up->OnHovered.AddDynamic(this, &UUI_GameSettingBox::UpBtnHovered);

	Super::NativeConstruct();
}

void UUI_GameSettingBox::SetSettingText(FString _SettingName)
{
	Txt_SettingProperty->SetText(FText::FromString(_SettingName));
}

void UUI_GameSettingBox::InitSettingProperty(ESlateVisibility _Visibility)
{
	if ( _Visibility == ESlateVisibility::SelfHitTestInvisible )
	{
		switch ( Setting_Property )
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
	}
	else
	{
		UGameUserSettings::GetGameUserSettings()->LoadSettings();
		UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
		pGameInst->LoadMasterVolume();
	}
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
		UE_LOG(LogTemp, Warning, TEXT("EWindowMode::전체화면"));
		PropertyVal = L"전체화면";
		break;
	case EWindowMode::Windowed:
		UE_LOG(LogTemp, Warning, TEXT("EWindowMode::창 모드"));
		PropertyVal = L"창 모드";
		break;
	case EWindowMode::WindowedFullscreen:
		UE_LOG(LogTemp, Warning, TEXT("EWindowMode::전체 창 모드"));
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

	if ( GetWorld()->WorldType == EWorldType::Game )
	{
		UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	}

	return Mode;
}

FIntPoint UUI_GameSettingBox::ChangeScreenResolution(FIntPoint _Res, bool _UpDown)
{
	FIntPoint ChangeRes;

	if ( GetWorld()->WorldType == EWorldType::Game )
	{
		UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
		ChangeRes = pGameInst->GetTempResolution();
	}

	UE_LOG(LogTemp, Warning, TEXT("current resolution : %d x %d"), _Res.X, _Res.Y);
	switch ( _Res.X )
	{
	case 1280:
		ChangeRes.X = _UpDown ? 1600 : 1280;
		ChangeRes.Y = _UpDown ? 900  : 720;
		break;
	case 1600:
		ChangeRes.X = _UpDown ? 1920 : 1280;
		ChangeRes.Y = _UpDown ? 1080 : 720;
		break;
	case 1920:
		ChangeRes.X = _UpDown ? 2560 : 1600;
		ChangeRes.Y = _UpDown ? 1440 : 900;
		break;
	case 2560:
		ChangeRes.X = _UpDown ? 2560 : 1920;
		ChangeRes.Y = _UpDown ? 1440 : 1080;
		break;
	default:
		ChangeRes.X = _UpDown ? 1920 : 1280;
		ChangeRes.Y = _UpDown ? 1080 : 720;
	}

	if (GetWorld()->WorldType == EWorldType::Game)
	{
		UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
		pGameInst->SetTempResolution(FIntPoint(ChangeRes.X, ChangeRes.Y));
	}

	UE_LOG(LogTemp, Warning, TEXT("new resolution : %d x %d"), ChangeRes.X, ChangeRes.Y);
	return FIntPoint(ChangeRes.X, ChangeRes.Y);
}

void UUI_GameSettingBox::DownBtnClicked()
{
	PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));

	switch ( Setting_Property )
	{
	case ESETTING_PROPERTY::SCREEN_MODE:
		{
			UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
			EWindowMode::Type ScreenMode = ChangeScreenMode(pGameInst->GetTempWindowMode(), false);
			SetUserSettingValText(ScreenMode);
			pGameInst->SetTempWindowMode(ScreenMode);
			//EWindowMode::Type ScreenMode = ChangeScreenMode(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode(), false);
			//UGameUserSettings::GetGameUserSettings()->SetScreenResolution(UGameUserSettings::GetGameUserSettings()->GetScreenResolution());
			//UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(ScreenMode);
			break;
		}
	case ESETTING_PROPERTY::RESOLUTION:
		{
			UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
			FIntPoint Resolution = ChangeScreenResolution(pGameInst->GetTempResolution(), false);
			SetUserSettingValText(Resolution);
			pGameInst->SetTempResolution(Resolution);
			//FIntPoint Resolution = ChangeScreenResolution(UGameUserSettings::GetGameUserSettings()->GetScreenResolution(), false);
			//UGameUserSettings::GetGameUserSettings()->SetScreenResolution(Resolution);
			//UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode());
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
	PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));
}

void UUI_GameSettingBox::UpBtnClicked()
{
	PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));

	switch ( Setting_Property )
	{
	case ESETTING_PROPERTY::SCREEN_MODE:
		{
			UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
			EWindowMode::Type ScreenMode = ChangeScreenMode(pGameInst->GetTempWindowMode(), true);
			SetUserSettingValText(ScreenMode);
			pGameInst->SetTempWindowMode(ScreenMode);
			//EWindowMode::Type ScreenMode = ChangeScreenMode(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode(), true);
			//UGameUserSettings::GetGameUserSettings()->SetScreenResolution(UGameUserSettings::GetGameUserSettings()->GetScreenResolution());
			//UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(ScreenMode);
			break;
		}
	case ESETTING_PROPERTY::RESOLUTION:
		{
			UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
			FIntPoint Resolution = ChangeScreenResolution(pGameInst->GetTempResolution(), true);
			SetUserSettingValText(Resolution);
			pGameInst->SetTempResolution(Resolution);
			//FIntPoint Resolution = ChangeScreenResolution(UGameUserSettings::GetGameUserSettings()->GetScreenResolution(), true);
			//UGameUserSettings::GetGameUserSettings()->SetScreenResolution(Resolution);
			//UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(UGameUserSettings::GetGameUserSettings()->GetFullscreenMode());
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
	PlaySound(GETMENUSOUND(EMenuSound::MENU_CHANGE));
}