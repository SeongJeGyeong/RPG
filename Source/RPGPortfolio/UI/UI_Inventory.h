// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Inventory.generated.h"

class UTextBlock;
class UButton;
class UImage;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTileView*		m_ItemTileView;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_ItemTooltip*	m_ItemTooltipUI;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*				m_CategoryText;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*				m_ItemName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_PlayerStat*	m_PlayerStatUI;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*				m_Btn_Cat_Left;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*				m_Btn_Cat_Right;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage*					m_Category_Img;

	EITEM_TYPE			eCategory;

public:
	void BindInvenMgr();
	void InventoryOpen(bool _Open);
	void AddInventoryItem(UObject* _ItemData);
	void Clear();
	bool IsInventoryOpened();
	void SetStatUI();
	void SetCategoryEnum(const EITEM_TYPE _Type) { eCategory = _Type; }
	void SetCategoryUI(const EITEM_TYPE _Type);

public:
	virtual void NativeConstruct() override;
	void OnTileHovered(UObject* _ItemData, bool _Hovered);

	UFUNCTION()
	void LeftBtnClicked();
	UFUNCTION()
	void RightBtnClicked();
};
