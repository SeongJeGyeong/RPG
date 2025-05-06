// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ItemTooltip.generated.h"

class UTextBlock;
class UImage;
class UItem_InvenData;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_ItemTooltip : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_ItemName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_Category;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_Sub_Category;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_CurQnt_Inven;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_MaxQnt_Inven;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_CurQnt_Storage;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_MaxQnt_Storage;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_PhysicAtk;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_PhysicDef;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_MagicAtk;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_MagicDef;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Restore_HP;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Restore_MP;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Item_Desc;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_Req_Str;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_Req_Dex;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock*	m_Sel_Req_Int;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage*		m_Sel_ItemImg;

public:
	virtual void NativeConstruct() override;

	void SetTooltipUI(UItem_InvenData* _InvenData);
	void SetTooltipUI(FGameItemInfo* _Info, uint32 Stack);

	void SetSelectedCategoryText(const EITEM_TYPE& _Type);
	FText GetStatText(const float& _StatVal);
};
