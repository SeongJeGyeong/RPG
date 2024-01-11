// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Message_Main.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Message_Main : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUI_Message_Main* GetUI_Message_Main() { return this; }

};
