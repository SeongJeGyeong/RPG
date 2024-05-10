// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "../System/Interface/PlayerInteraction.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Trigger_Interaction.generated.h"

/**
 * 
 */
UCLASS(meta = ( BlueprintSpawnableComponent ))
class RPGPORTFOLIO_API UTrigger_Interaction : public UBoxComponent, public IPlayerInteraction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type", meta = ( AllowPrivateAccess = "true" ))
	EInteractionType TriggerType;

public:
	UTrigger_Interaction();

	virtual void Interaction() override;
	virtual FText GetCommand_Key() const override;
	virtual FText GetCommand_Name() const override;
};
