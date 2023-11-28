// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UI/UI_TitleScreen.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATitleGameMode();
	~ATitleGameMode();

private:
	TSubclassOf<UUI_TitleScreen>	m_TitleScreenClass;
	UUI_TitleScreen*				m_TitleScreen;

public:
	virtual void BeginPlay() override;

};
