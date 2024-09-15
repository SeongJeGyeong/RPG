// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI_InvenItem.generated.h"

class UTextBlock;
class UImage;
class UButton;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_InvenItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_ItemQnt;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage*		m_ItemImg;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage*		m_EquipMark;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*	m_ItemBtn;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UMenuAnchor* m_ItemMenuAnchor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	class UItem_InvenData* m_ItemData;

	bool bItemUseDelay;
	bool bAnchorActive;

	EEQUIP_SLOT	eSelectedSlot = EEQUIP_SLOT::EMPTY;

	// 아이템 장착시 변경될 능력치 표시용
	class UUI_PlayerStat* m_StatUI;

public:
	UFUNCTION()
	void InitFromData(UObject* _Data);

	void SetAnchorActive(bool _AnchorActive) { bAnchorActive = _AnchorActive; }
	void SetSelectedSlot(EEQUIP_SLOT _SelectedSlot) { eSelectedSlot = _SelectedSlot; }
	void SetStatUI(UUI_PlayerStat* _StatUI) { m_StatUI = _StatUI; }
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void ItemBtnClicked();

	UFUNCTION()
	UUserWidget* MenuAnchorDataSetting();
};
