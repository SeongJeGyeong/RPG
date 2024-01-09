// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item_InvenData.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UItem_InvenData : public UObject
{
	GENERATED_BODY()
	
private:
	FString	m_ItemName;
	FString	m_ItemImgPath;
	FString	m_ItemDesc;
	uint16	m_ItemQnt;

public:
	const FString& GetItemName() { return m_ItemName; }
	void SetItemName(const FString& _ItemName) { m_ItemName = _ItemName; }

	const FString& GetItemImgPath() { return m_ItemImgPath; }
	void SetItemImgPath(const FString& _ItemImgPath) { m_ItemImgPath = _ItemImgPath; }

	const FString& GetItemDesc() { return m_ItemDesc; }
	void SetItemDesc(const FString& _ItemDesc) { m_ItemDesc = _ItemDesc; }

	const uint16& GetItemQnt() { return m_ItemQnt; }
	void SetItemQnt(const uint16& _ItemQnt) { m_ItemQnt = _ItemQnt; }

};
