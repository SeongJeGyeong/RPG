// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ItemData.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_ItemData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Item", Meta = (DisplayName = "ItemData"))
	TArray<FGameItemData> ItemDataArr;

};
