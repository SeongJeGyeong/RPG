// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()

private:
	class UUI_Player_Main* m_MainUI;
	class UUI_Player_QuickSlot* m_QuickSlotUI;
	class UUI_Player_Soul* m_SoulUI;
	class UUI_Menu_Main* m_MenuUI;
	class UUI_Boss* m_BossUI;

public:
	class UUI_Player_Main* GetMainUIWidget() { return m_MainUI; }

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

};
