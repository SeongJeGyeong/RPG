// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_InputAction.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_InputAction : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (DisplayName = "InputAction Setting"))
	TArray<FInputActionData>	IADataArr;

};
