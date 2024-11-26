// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Player_QuickSlotItem.generated.h"

class UImage;
class UTextBlock;
class UItem_InvenData;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Player_QuickSlotItem : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage*		m_Dish;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage*		m_ItemImg;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_ItemName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Amount;

public:
	virtual void NativeConstruct() override;

	void RenewQuickSlotItem(UItem_InvenData* const _InvenItem);
	void RenewNextQuickSlotItem(UItem_InvenData* const _InvenItem);
};
