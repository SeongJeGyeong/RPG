// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance_Base.h"
#include "Manager/Inventory_Mgr.h"
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
	ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if (ItemTable.Succeeded())
	{
		UInventory_Mgr::GetInst(this)->SetItemDataTable(ItemTable.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 데이터테이블 찾지 못함"));
	}

	ConstructorHelpers::FClassFinder<UUserWidget> loadingscreen(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_LoadingScreen.UI_LoadingScreen_C'"));
	if ( loadingscreen.Succeeded() )
	{
		m_LoadingScreenClass = loadingscreen.Class;
	}

	ConstructorHelpers::FObjectFinder<USoundClass> soundclass(TEXT("/Script/Engine.SoundClass'/Game/Audio/Classes/Overall.Overall'"));
	if ( loadingscreen.Succeeded() )
	{
		m_MasterVolume = soundclass.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> CharacterSheet(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_CharacterSheet.DT_CharacterSheet'"));
	if ( CharacterSheet.Succeeded() || CharacterSheet.Object->GetRowStruct()->IsChildOf(FCharacterStatSheet::StaticStruct()) )
	{
		FCharacterStatSheet* pCharacterStat;
		pCharacterStat = CharacterSheet.Object->FindRow<FCharacterStatSheet>(FName("Knight"), TEXT(""));
		PlayerStat = *pCharacterStat;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> CharacterSoul(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_PlayerAmountOfSoul.DT_PlayerAmountOfSoul'"));
	if ( CharacterSoul.Succeeded() )
	{
		PlayerBasePower.AmountOfSoul = CharacterSoul.Object->FindRow<FPlayerAmountOfSoul>(FName("Knight"), TEXT(""))->AmountOfSoul;
	}

	PlayerBasePower.MaxHP = PlayerStat.Vigor * 100.f;
	PlayerBasePower.CurHP = PlayerBasePower.MaxHP;
	PlayerBasePower.MaxMP = PlayerStat.Attunement * 10.f;
	PlayerBasePower.CurMP = PlayerBasePower.MaxMP;
	PlayerBasePower.MaxStamina = PlayerStat.Endurance * 10.f;
	PlayerBasePower.CurStamina = PlayerBasePower.MaxStamina;
	PlayerBasePower.PhysicAtk = ( ( PlayerStat.Strength + PlayerStat.Dexterity ) / 2 ) * 20.f;
	PlayerBasePower.PhysicDef = ( ( PlayerStat.Strength + PlayerStat.Dexterity ) / 2 ) * 10.f;
	PlayerBasePower.MagicAtk = PlayerStat.Intelligence * 20.f;
	PlayerBasePower.MagicDef = ( ( PlayerStat.Attunement + PlayerStat.Intelligence ) / 2 ) * 10.f;
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
}

float UGameInstance_Base::GetMasterVolume() const
{
	return fTempVolume;
}

void UGameInstance_Base::SetMasterVolume(const float& _Volume)
{
	fTempVolume = _Volume;
}

void UGameInstance_Base::ApplyMasterVolume()
{
	m_MasterVolume->Properties.Volume = fTempVolume;
}

void UGameInstance_Base::ExecuteResoltionCommand()
{
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
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand(*SetCommand);
}

void UGameInstance_Base::BeginLoadingScreen(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), m_LoadingScreenClass);
	LoadingScreen.WidgetLoadingScreen = Widget->TakeWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void UGameInstance_Base::EndLoadingScreen(UWorld* InLoadedWorld)
{
	//ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//if ( IsValid(GameMode) )
	//{
	//	GameMode->GetFadeUI()->FadeIn(2.f);
	//}

	const UWorld* World = GetWorld();
	if ( World )
	{
		UFadeViewportClient* GameViewportClient = Cast<UFadeViewportClient>(World->GetGameViewport());
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
