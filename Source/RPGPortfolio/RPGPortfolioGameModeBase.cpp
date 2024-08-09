// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGPortfolioGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/UI_Base.h"
#include "UI/UI_Inventory.h"
#include "UI/UI_StatusMain.h"
#include "UI/UI_EquipMain.h"
#include "UI/UI_Manual.h"
#include "UI/UI_Settings.h"
#include "UI/UI_FadeScreen.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"

ARPGPortfolioGameModeBase::ARPGPortfolioGameModeBase()
{
	ConstructorHelpers::FClassFinder<UUserWidget> mainHUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (mainHUD.Succeeded())
	{
		m_WidgetClassArr.Add(mainHUD.Class);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Inventory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Inventory/BPC_UI_Inventory.BPC_UI_Inventory_C'"));
	if (Inventory.Succeeded())
	{
		m_WidgetClassArr.Add(Inventory.Class);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Status(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Status/BPC_UI_Status.BPC_UI_Status_C'"));
	if (Status.Succeeded())
	{
		m_WidgetClassArr.Add(Status.Class);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Equip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Equip/BPC_UI_Equip.BPC_UI_Equip_C'"));
	if (Equip.Succeeded())
	{
		m_WidgetClassArr.Add(Equip.Class);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Manual(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Manual/BPC_UI_Manual.BPC_UI_Manual_C'"));
	if (Manual.Succeeded())
	{
		m_WidgetClassArr.Add(Manual.Class);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Settings(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Settings/BPC_UI_Settings.BPC_UI_Settings_C'"));
	if (Settings.Succeeded())
	{
		m_WidgetClassArr.Add(Settings.Class);
	}

	ConstructorHelpers::FClassFinder<UUserWidget> FadeScreen(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/BPC_UI_FadeScreen.BPC_UI_FadeScreen_C'"));
	if ( FadeScreen.Succeeded() )
	{
		m_WidgetClassArr.Add(FadeScreen.Class);
	}

	m_BGMComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	m_BGMComp->SetupAttachment(GetRootComponent());
	m_BGMComp->bAutoActivate = false;
	m_BGMComp->SetUISound(true);
}

ARPGPortfolioGameModeBase::~ARPGPortfolioGameModeBase()
{

}

void ARPGPortfolioGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GEngine->GetGameUserSettings()->SetViewDistanceQuality(1);
	GEngine->GetGameUserSettings()->SetAntiAliasingQuality(1);
	GEngine->GetGameUserSettings()->SetPostProcessingQuality(1);
	GEngine->GetGameUserSettings()->SetShadowQuality(1);
	GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(1);
	GEngine->GetGameUserSettings()->SetReflectionQuality(1);
	GEngine->GetGameUserSettings()->SetTextureQuality(1);
	GEngine->GetGameUserSettings()->SetVisualEffectQuality(1);
	GEngine->GetGameUserSettings()->SetFoliageQuality(1);
	GEngine->GetGameUserSettings()->SetShadingQuality(1);

	if (IsValid(m_WidgetClassArr[0]))
	{
		m_MainHUD = Cast<UUI_Base>(CreateWidget(GetWorld(), m_WidgetClassArr[0]));

		if (IsValid(m_MainHUD))
		{
			m_MainHUD->AddToViewport(1);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UI_Base 캐스팅 실패"));
		}
	}

	if (IsValid(m_WidgetClassArr[1]))
	{
		m_InventoryUI = Cast<UUI_Inventory>(CreateWidget(GetWorld(), m_WidgetClassArr[1]));

		if (IsValid(m_InventoryUI))
		{
			m_InventoryUI->AddToViewport(5);
			m_InventoryUI->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UI_Inventory 캐스팅 실패"));
		}
	}

	if (IsValid(m_WidgetClassArr[2]))
	{
		m_StatusUI = Cast<UUI_StatusMain>(CreateWidget(GetWorld(), m_WidgetClassArr[2]));
		
		if (IsValid(m_StatusUI))
		{
			m_StatusUI->AddToViewport(5);
			m_StatusUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (IsValid(m_WidgetClassArr[3]))
	{
		m_EquipUI = Cast<UUI_EquipMain>(CreateWidget(GetWorld(), m_WidgetClassArr[3]));
		if (IsValid(m_EquipUI))
		{
			m_EquipUI->AddToViewport(5);
			m_EquipUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if ( IsValid(m_WidgetClassArr[4]) )
	{
		m_ManualUI = Cast<UUI_Manual>(CreateWidget(GetWorld(), m_WidgetClassArr[4]));

		if ( IsValid(m_ManualUI) )
		{
			m_ManualUI->AddToViewport(5);
			m_ManualUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if ( IsValid(m_WidgetClassArr[5]) )
	{
		m_SettingsUI = Cast<UUI_Settings>(CreateWidget(GetWorld(), m_WidgetClassArr[5]));

		if ( IsValid(m_SettingsUI) )
		{
			m_SettingsUI->AddToViewport(5);
			m_SettingsUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if ( IsValid(m_WidgetClassArr[6]) )
	{
		m_FadeScreenUI = Cast<UUI_FadeScreen>(CreateWidget(GetWorld(), m_WidgetClassArr[6]));

		if ( IsValid(m_FadeScreenUI) )
		{
			m_FadeScreenUI->AddToViewport(6);
			m_FadeScreenUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	APlayerController* pController = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly GameOnly;
	pController->SetInputMode(GameOnly);
	pController->bShowMouseCursor = false;
}

bool ARPGPortfolioGameModeBase::IsSubMenuUIOpened()
{
	if (m_InventoryUI->GetVisibility() == ESlateVisibility::Visible ||
		m_StatusUI->GetVisibility() == ESlateVisibility::Visible ||
		m_EquipUI->GetVisibility() == ESlateVisibility::Visible	||
		m_ManualUI->GetVisibility() == ESlateVisibility::Visible ||
		m_SettingsUI->GetVisibility() == ESlateVisibility::Visible
		)
	{
		return true;
	}

	return false;
}

void ARPGPortfolioGameModeBase::CloseSubMenu()
{
	m_InventoryUI->SetVisibility(ESlateVisibility::Hidden);
	m_StatusUI->SetVisibility(ESlateVisibility::Hidden);

	if (m_EquipUI->GetVisibility() == ESlateVisibility::Visible)
	{
		if (m_EquipUI->GetItemListVisibility())
		{
			m_EquipUI->CloseItemList();
		}
		else
		{
			m_EquipUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	m_ManualUI->SetVisibility(ESlateVisibility::Hidden);
	m_SettingsUI->SetVisibility(ESlateVisibility::Hidden);
}

void ARPGPortfolioGameModeBase::PlayBGM(bool _Play)
{
	if (_Play)
	{
		USoundBase* pBGM = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Blueprint/Monster/Sound/GreaterSpider/SC_GS_BGM.SC_GS_BGM'"));
		m_BGMComp->SetSound(pBGM);
		m_BGMComp->Play();
	}
	else
	{
		m_BGMComp->Stop();
	}
}
