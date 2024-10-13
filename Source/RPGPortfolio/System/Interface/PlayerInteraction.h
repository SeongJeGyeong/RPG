// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInteraction.generated.h"
UINTERFACE(MinimalAPI)
class UPlayerInteraction : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPORTFOLIO_API IPlayerInteraction
{
	GENERATED_BODY()

public:
	virtual void Interaction(AActor* _InteractedActor = nullptr) = 0;
	virtual FText GetCommand_Key() const = 0;
	virtual FText GetCommand_Name() const = 0;
};
