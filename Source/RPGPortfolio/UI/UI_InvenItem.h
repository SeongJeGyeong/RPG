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
	class UMenuAnchor*	m_MenuAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	class UItem_InvenData* m_ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	bool bItemUseDelay;

	bool bAnchorActive;
	EEQUIP_SLOT	eSelectedSlot = EEQUIP_SLOT::EMPTY;

	// 아이템 장착시 변경될 능력치 표시용
	class UUI_PlayerStat* m_StatUI;

	class UDA_MenuSound* m_Sound;

public:
	UFUNCTION()
	void InitFromData(UObject* _Data);

	void SetAnchorActive(bool _AnchorActive) { bAnchorActive = _AnchorActive; }
	void SetSelectedSlot(EEQUIP_SLOT _SelectedSlot) { eSelectedSlot = _SelectedSlot; }
	void SetStatUI(UUI_PlayerStat* _StatUI) { m_StatUI = _StatUI; }
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void ItemBtnClicked();

	UFUNCTION()
	void MenuAnchorOpened();
};
