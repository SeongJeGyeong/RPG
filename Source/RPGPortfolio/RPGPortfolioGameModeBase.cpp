// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGPortfolioGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/UI_Base.h"
#include "UI/UI_Inventory.h"

ARPGPortfolioGameModeBase::ARPGPortfolioGameModeBase()
{
	ConstructorHelpers::FClassFinder<UUserWidget> mainHUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (mainHUD.Succeeded())
	{
		m_MainHUDClass = mainHUD.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Inventory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Inventory/BPC_UI_Inventory.BPC_UI_Inventory_C'"));
	if (Inventory.Succeeded())
	{
		m_InventoryUIClass = Inventory.Class;
	}
}

ARPGPortfolioGameModeBase::~ARPGPortfolioGameModeBase()
{

}

void ARPGPortfolioGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(m_MainHUDClass))
	{
		m_MainHUD = Cast<UUI_Base>(CreateWidget(GetWorld(), m_MainHUDClass));

		if (IsValid(m_MainHUD))
		{
			m_MainHUD->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UI_Base 캐스팅 실패"));
		}
	}

	if ( IsValid(m_InventoryUIClass) )
	{
		m_InventoryUI = Cast<UUI_Inventory>(CreateWidget(GetWorld(), m_InventoryUIClass));

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

	APlayerController* pController = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly GameOnly;
	pController->SetInputMode(GameOnly);
	pController->bShowMouseCursor = false;
}


