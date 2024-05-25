// Fill out your copyright notice in the Description page of Project Settings.


#include "LS_Base.h"
#include "Kismet/GameplayStatics.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../UI/UI_Base.h"

void ULS_Base::FadeInHUD()
{
	fOpacityRate = FMath::Clamp(fOpacityRate + 0.04f, 0.f, 1.f);
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		GameMode->GetMainHUD()->SetRenderOpacity(fOpacityRate);
	}
	UE_LOG(LogTemp, Warning, TEXT("UI Fade In"));
}

void ULS_Base::FadeOutHUD()
{
	fOpacityRate = FMath::Clamp(fOpacityRate - 0.04f, 0.f, 1.f);
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if ( IsValid(GameMode) )
	{
		GameMode->GetMainHUD()->SetRenderOpacity(fOpacityRate);
	}
	UE_LOG(LogTemp, Warning, TEXT("UI Fade Out"));
}
