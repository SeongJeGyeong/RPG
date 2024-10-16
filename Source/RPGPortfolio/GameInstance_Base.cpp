// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance_Base.h"
#include "Manager/Inventory_Mgr.h"
#include "Manager/Equip_Mgr.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "RPGPortfolioGameModeBase.h"
#include "UI/UI_FadeScreen.h"
#include "Kismet/GameplayStatics.h"
#include "System/FadeViewportClient.h"
#include "GameFramework/GameUserSettings.h"

UGameInstance_Base::UGameInstance_Base()
	: m_InvenMgr(nullptr)
	, m_EquipMgr(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if (ItemTable.Succeeded())
	{
		UInventory_Mgr::GetInst(this)->SetItemDataTable(ItemTable.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 데이터테이블 찾지 못함"));
	}

	static ConstructorHelpers::FObjectFinder<UDataAsset> InvenIcon(TEXT("/Script/RPGPortfolio.DA_ItemCategoryIcon'/Game/Blueprint/DataAsset/BPC_DA_CategoryIcon.BPC_DA_CategoryIcon'"));
	if ( InvenIcon.Succeeded() )
	{
		UInventory_Mgr::GetInst(this)->SetInventoryIcon(InvenIcon.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("아이콘 데이터에셋 찾지 못함"));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> loadingscreen(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_LoadingScreen.UI_LoadingScreen_C'"));
	if ( loadingscreen.Succeeded() )
	{
		m_LoadingScreenClass = loadingscreen.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundClass> soundclass(TEXT("/Script/Engine.SoundClass'/Game/Audio/Classes/Overall.Overall'"));
	if ( loadingscreen.Succeeded() )
	{
		m_MasterVolume = soundclass.Object;
	}
}

UGameInstance_Base::~UGameInstance_Base()
{
}

void UGameInstance_Base::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UGameInstance_Base::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameInstance_Base::EndLoadingScreen);
	
	fTempVolume = m_MasterVolume->Properties.Volume;
	TempResolution = UGameUserSettings::GetGameUserSettings()->GetScreenResolution();
	TempWindowMode = UGameUserSettings::GetGameUserSettings()->GetFullscreenMode();
}

float UGameInstance_Base::GetMasterVolume() const
{
	return fTempVolume;
}

void UGameInstance_Base::SetMasterVolume(const float& _Volume)
{
	fTempVolume = _Volume;
}

void UGameInstance_Base::LoadMasterVolume()
{
	fTempVolume = m_MasterVolume->Properties.Volume;
}

void UGameInstance_Base::ApplyMasterVolume()
{
	m_MasterVolume->Properties.Volume = fTempVolume;
}

void UGameInstance_Base::ExecuteResoltionCommand()
{
	// 스탠드얼론용 GameUserSettings 파일이 없어 세팅이 제대로 설정되이 않으므로 콘솔명령으로 대체
	FString SetCommand;
	
	switch (UGameUserSettings::GetGameUserSettings()->GetFullscreenMode())
	{
	case EWindowMode::Fullscreen:
		SetCommand = L"r.SetRes " + FString::Printf(TEXT("%dx%d"), TempResolution.X, TempResolution.Y) + L"f";
		break;
	case EWindowMode::WindowedFullscreen:
		SetCommand = L"r.SetRes " + FString::Printf(TEXT("%dx%d"), TempResolution.X, TempResolution.Y) + L"wf";
		break;
	case EWindowMode::Windowed:
		SetCommand = L"r.SetRes " + FString::Printf(TEXT("%dx%d"), TempResolution.X, TempResolution.Y) + L"w";
		break;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Command : %s"), *SetCommand);
	GEngine->Exec(GetWorld(), *SetCommand);
}

void UGameInstance_Base::RestartPlayer()
{
	UFadeViewportClient* GameViewportClient = Cast<UFadeViewportClient>(GetWorld()->GetGameViewport());
	if ( GameViewportClient )
	{
		GameViewportClient->Fade(1.f, true);
	}

	FTimerHandle RestartTimer;
	GetWorld()->GetTimerManager().SetTimer(RestartTimer, FTimerDelegate::CreateWeakLambda(this, [this]
		{
			FString LevelName = GetWorld()->GetMapName();
			LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
			UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
		}
	)
	, 2.f, false, 2.f);
}

void UGameInstance_Base::BeginLoadingScreen(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreen.MinimumLoadingScreenDisplayTime = 1.f;
	LoadingScreen.bMoviesAreSkippable = false;
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), m_LoadingScreenClass);
	LoadingScreen.WidgetLoadingScreen = Widget->TakeWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void UGameInstance_Base::EndLoadingScreen(UWorld* InLoadedWorld)
{
	if ( InLoadedWorld )
	{
		UFadeViewportClient* GameViewportClient = Cast<UFadeViewportClient>(InLoadedWorld->GetGameViewport());
		if ( GameViewportClient )
		{
			GameViewportClient->Fade(1.f, false);
		}
	}
}

void UGameInstance_Base::ASyncLoadDataAsset(FSoftObjectPath _AssetPath)
{
	UE_LOG(LogTemp, Warning, TEXT("DataAsset Path : %s"), *_AssetPath.ToString());
	UE_LOG(LogTemp, Warning, TEXT("DataAsset Load Start : %s"), *_AssetPath.GetAssetName());

	TSharedPtr<FStreamableHandle> StreamHandle = AssetStreamManager.RequestAsyncLoad(_AssetPath, FStreamableDelegate::CreateUObject(this, &UGameInstance_Base::AssetLoaded, _AssetPath.GetAssetName()));
	GetWorld()->GetTimerManager().SetTimer(StreamTimer, [this, StreamHandle, _AssetPath]
		{
			if ( AssetStreamManager.IsAsyncLoadComplete(_AssetPath) )
			{
				UE_LOG(LogTemp, Warning, TEXT("AsyncLoadComplete"));
				GetWorld()->GetTimerManager().ClearTimer(StreamTimer);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("AsyncLoading"));
			}
		}
	, 0.1f, true);
}

void UGameInstance_Base::AssetLoaded(FString _AssetName)
{
	UE_LOG(LogTemp, Warning, TEXT("DataAsset Loaded : %s"), *_AssetName);
}
