// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGPortfolioGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/UI_Base.h"

ARPGPortfolioGameModeBase::ARPGPortfolioGameModeBase()
{
	ConstructorHelpers::FClassFinder<UUserWidget> mainHUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (mainHUD.Succeeded())
	{
		m_MainHUDClass = mainHUD.Class;
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
			UE_LOG(LogTemp, Error, TEXT("UI_Base Casting Failed"));
		}
	}

	APlayerController* pController = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly GameOnly;
	pController->SetInputMode(GameOnly);
	pController->bShowMouseCursor = false;
}


