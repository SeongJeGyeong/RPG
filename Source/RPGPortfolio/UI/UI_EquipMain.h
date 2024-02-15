// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipMain.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_EquipMain : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock*		m_SelItemName;
	class UTextBlock*		m_SelItemSlot;
	class UUI_ItemTooltip*	m_Tooltip;
	class UUI_PlayerStat*	m_Stat;
	class UUI_EquipItemList* m_ItemList;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void ListVisibilityChanged(ESlateVisibility _Visibility);

public:
	UUI_EquipItemList* GetItemList() { return m_ItemList; }
};
