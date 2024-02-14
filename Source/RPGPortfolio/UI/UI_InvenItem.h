// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI_InvenItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_InvenItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
private:
	class UTextBlock*	m_ItemName;
	class UTextBlock*	m_ItemDesc;
	class UTextBlock*	m_ItemQnt;
	class UImage*		m_ItemImg;
	class UImage*		m_EquipMark;
	class UButton*		m_ItemBtn;
	class UItem_InvenData* m_ItemData;
	class UMenuAnchor*	m_MenuAnchor;

	bool bAnchorActive;
	EEQUIP_SLOT	eSelectedSlot = EEQUIP_SLOT::EMPTY;

public:
	UFUNCTION()
	void InitFromData(UObject* _Data);

	void SetAnchorActive(bool _AnchorActive) { bAnchorActive = _AnchorActive; }
	void SetSelectedSlot(EEQUIP_SLOT _SelectedSlot) { eSelectedSlot = _SelectedSlot; }

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void ItemBtnClicked();
};
