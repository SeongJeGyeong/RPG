// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"

ATitleGameMode::ATitleGameMode()
{
	ConstructorHelpers::FClassFinder<UUserWidget> TitleScreen(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_Title.UI_Title_C'"));
	if (TitleScreen.Succeeded())
	{
		m_TitleScreenClass = TitleScreen.Class;
	}
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_TitleScreen = Cast<UUI_TitleScreen>(CreateWidget(GetWorld(), m_TitleScreenClass));

	if (!IsValid(m_TitleScreen))
	{
		UE_LOG(LogTemp, Error, TEXT("Title Screen Load Failed"));
	}
	else
	{
		m_TitleScreen->AddToViewport();

		APlayerController* pController = GetWorld()->GetFirstPlayerController();

		FInputModeUIOnly UIOnly;
		pController->SetInputMode(UIOnly);
		pController->bShowMouseCursor = true;
	}
}
