// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ItemSelectMenu.generated.h"

class UTextBlock;
class UButton;
class UItem_InvenData;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_ItemSelectMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*		m_Txt_Use;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*		m_Btn_Use;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*		m_Btn_Drop;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*		m_Btn_Discard;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*		m_Btn_DropAll;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton*		m_Btn_DiscardAll;

	EITEM_ID		m_ID = EITEM_ID::END;
	EEQUIP_SLOT		m_Slot = EEQUIP_SLOT::EMPTY;
	EITEM_TYPE		m_Type = EITEM_TYPE::END;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	bool bItemUseDelay;

public:
	virtual void NativeConstruct() override;

public:
	//void SetSelectedItemData(UItem_InvenData* _ItemData) { m_SelectedItemData = _ItemData; }
	void SetSelectedItemID(EITEM_ID _ID) { m_ID = _ID; }
	void SetSelectedItemType(EITEM_TYPE _Type) { m_Type = _Type; }
	void SetSelectedItemSlot(EEQUIP_SLOT _Slot) { m_Slot = _Slot; }
	void SetbItemUseDelay(const bool& _ItemUseDelay) { bItemUseDelay = _ItemUseDelay; }

	UFUNCTION()
	void UseBtnClicked();
};
