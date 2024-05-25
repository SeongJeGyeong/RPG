// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "LS_Base.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class RPGPORTFOLIO_API ULS_Base : public ULevelSequence
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	float fOpacityRate = 0.f;

public:
	UFUNCTION(BlueprintCallable)
	void FadeInHUD();
	UFUNCTION(BlueprintCallable)
	void FadeOutHUD();
};
