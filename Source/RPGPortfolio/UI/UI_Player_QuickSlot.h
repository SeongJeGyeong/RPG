// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Player_QuickSlot.generated.h"

class UUI_Player_QuickSlotItem;
class UProgressBar;
class UItem_InvenData;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Player_QuickSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UUI_Player_QuickSlotItem*	m_UI_UpperSlot;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UUI_Player_QuickSlotItem*	m_UI_LowerSlot;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UUI_Player_QuickSlotItem*	m_UI_NextQuickSlot;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UProgressBar*				m_LowerSlot_DelayBar;

public:
	virtual void NativeConstruct() override;

	void RenewLowerQuickSlot(UItem_InvenData* _InvenItem);
	void RenewNextLowerQuickSlot(UItem_InvenData* _InvenItem);

	/*	false면 아래쪽 슬롯 투명도 세팅
		true면 위쪽 슬롯의 투명도 세팅	*/
	void SetQuickSlotOpacity(float Alpha, bool UorL);
	void SetLowerSlotDelay(float _DelayPercent);
	void PlayQuickSlotAnimation();

};
