// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance_Base.h"
#include "Manager/Inventory_Mgr.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "System/FadeViewportClient.h"

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
}

UGameInstance_Base::~UGameInstance_Base()
{
}

void UGameInstance_Base::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UGameInstance_Base::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameInstance_Base::EndLoadingScreen);
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
