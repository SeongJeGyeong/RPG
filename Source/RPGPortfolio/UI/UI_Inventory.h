// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTileView*		m_TileView;
	class UUI_ItemTooltip*	m_Tooltip;
	class UTextBlock*		m_Category;
	class UTextBlock*		m_ItemName;
	class UUI_PlayerStat*	m_Status;
	class UButton*			m_LeftBtn;
	class UButton*			m_RightBtn;
	class UImage*			m_Category_Img;

	enum EITEM_TYPE			eCategory;

public:
	void AddInventoryItem(UObject* _ItemData);
	void Clear();
	bool IsInventoryOpened();
	void SetStatUI(APlayerState* _PlayerState);
	void SetCategoryUI(EITEM_TYPE _Type);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;
	void OnTileHovered(UObject* _ItemData, bool _Hovered);

	UFUNCTION()
	void LeftBtnClicked();
	UFUNCTION()
	void RightBtnClicked();
};
