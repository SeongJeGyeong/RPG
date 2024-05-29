// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Player_QuickSlot.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Player_QuickSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UUI_Player_QuickSlotItem* m_UpperSlotItem;
	class UUI_Player_QuickSlotItem* m_LowerSlotItem;
	class UUI_Player_QuickSlotItem* m_NextQuickSlotItem;
	class UProgressBar*				m_LowerSlotDelayBar;

	float fItemDelayTime = 0.f;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void RenewLowerQuickSlot(int32 _Idx);
	void RenewNextQuickSlot(int32 _Idx);
	
	
	/*	false면 아래쪽 슬롯 투명도 세팅
		true면 위쪽 슬롯의 투명도 세팅	*/
	void SetQuickSlotOpacity(float Alpha, bool UorL);
	void SetLowerSlotDelay(float _DelayPercent);
};
