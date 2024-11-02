// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Base.generated.h"

/**
 * 
 */
UCLASS(Meta=(DisableNativeTick))
class RPGPORTFOLIO_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Player_Main*		m_UI_PlayerBar;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Player_QuickSlot* m_UI_QuickSlotMain;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Player_Soul*		m_UI_Soul;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Menu_Main*		m_UI_MenuMain;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Boss*				m_UI_Boss;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Message_Main*		m_UI_MessageBox_Main;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Message_Item*		m_UI_MessageBox_Item;

	bool bDisplayRootMessage = false;

public:
	class UUI_Player_Main* GetMainUIWidget() { return m_UI_PlayerBar; }
	class UUI_Message_Main* GetMainMessageUI() { return m_UI_MessageBox_Main; }
	class UUI_Message_Item* GetItemMessageUI() { return m_UI_MessageBox_Item; }
	class UUI_Player_QuickSlot* GetQuickSlotUI() { return m_UI_QuickSlotMain; }
	class UUI_Player_Soul* GetSoulUI() { return m_UI_Soul; }
	class UUI_Boss* GetBossUI() { return m_UI_Boss; }

	bool GetRootMessageDisplayed() { return bDisplayRootMessage; }

	void BindStatManager(class UGISubsystem_StatMgr* _StatMgr);

	void MenuVisibility(ESlateVisibility _Visibility);
	bool IsOpendMenu();

	void SetMainMessageUI(FText _Command, FText _Action);
	void ShowMainMessageUI(bool _bShow);

	void ShowItemMessageUI(bool _bShow);

	void RenewAmountSoul(int32 _GainedSoul);
	void RenewUI_HP(float _CurRatio);
	void RenewUI_MP(float _CurRatio);
	void RenewUI_ST(float _CurRatio);

	void HUD_RenewQuickSlotUI(int32 _idx);
	void SetQuickSlotUIOpacity(float _alpha, bool _UorL);
	void SetQuickSlotUIDelay(float _DelayPercnet);

public:
	virtual void NativeConstruct() override;
};
