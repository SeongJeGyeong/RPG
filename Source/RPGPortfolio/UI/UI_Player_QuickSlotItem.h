// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Player_QuickSlotItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Player_QuickSlotItem : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UImage*		m_Dish;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UImage*		m_ItemImg;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock*	m_ItemName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock*	m_Amount;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void RenewQuickSlotItem(struct FInvenItemRow* _InvenItem);
	void RenewNextQuickSlotItem(FInvenItemRow* _InvenItem);

};
