// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSprite.h"
#include "../../Header/Enum.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ItemCategoryIcon.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_ItemCategoryIcon : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Image", Meta = ( DisplayName = "카테고리 아이콘 이미지" ))
	TMap<EITEM_TYPE, UPaperSprite*> m_ItemCategoryIcon;

public:
	UPaperSprite* GetCategoryIcon(EITEM_TYPE _Type) { return *m_ItemCategoryIcon.Find(_Type); }
};
