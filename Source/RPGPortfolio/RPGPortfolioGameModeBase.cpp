// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPGPortfolioGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/UI_Base.h"
#include "UI/UI_Inventory.h"
#include "UI/UI_StatusMain.h"
#include "UI/UI_EquipMain.h"

ARPGPortfolioGameModeBase::ARPGPortfolioGameModeBase()
{
	ConstructorHelpers::FClassFinder<UUserWidget> mainHUD(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (mainHUD.Succeeded())
	{
		m_MainHUDClass = mainHUD.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Inventory(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Inventory/BPC_UI_Inventory.BPC_UI_Inventory_C'"));
	if (Inventory.Succeeded())
	{
		m_InventoryUIClass = Inventory.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Status(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Status/BPC_UI_Status.BPC_UI_Status_C'"));
	if (Status.Succeeded())
	{
		m_StatusUIClass = Status.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> Equip(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Menu/Equip/BPC_UI_Equip.BPC_UI_Equip_C'"));
	if (Equip.Succeeded())
	{
		m_EquipUIClass = Equip.Class;
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
			m_MainHUD->AddToViewport(1);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UI_Base 캐스팅 실패"));
		}
	}

	if (IsValid(m_InventoryUIClass))
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

	if (IsValid(m_StatusUIClass))
	{
		m_StatusUI = Cast<UUI_StatusMain>(CreateWidget(GetWorld(), m_StatusUIClass));
		
		if (IsValid(m_StatusUI))
		{
			m_StatusUI->AddToViewport(5);
			m_StatusUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (IsValid(m_EquipUIClass))
	{
		m_EquipUI = Cast<UUI_EquipMain>(CreateWidget(GetWorld(), m_EquipUIClass));

		if (IsValid(m_EquipUIClass))
		{
			m_EquipUI->AddToViewport(5);
			m_EquipUI->SetVisibility(ESlateVisibility::Hidden);
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
		m_EquipUI->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}

	return false;
}