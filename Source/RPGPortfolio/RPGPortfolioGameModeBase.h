// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGPortfolioGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ARPGPortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ARPGPortfolioGameModeBase();
	~ARPGPortfolioGameModeBase();

private:
	TSubclassOf<UUserWidget> m_MainHUDClass;
	class UUI_Base* m_MainHUD;

public:
	class UUI_Base* GetMainHUD() { return m_MainHUD; }

public:
	virtual void BeginPlay() override;

};
