// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Player_Soul.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Player_Soul : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* m_AmountOfSoul;
	class UTextBlock* m_GainedSoul;
	class UDA_MenuSound* m_Sound;
	
	int32 iDisplayedSoul;
	int32 iGainedSoul;
	bool bSoulGained = false;
	bool bDisplayGainedSoul = false;
	bool bDoPlaySound = false;
	bool bDoOnce = false;

	float fDisplayTime = 0.f;
	float fOpacity = 1.f;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void RenewAmountOfSoul(int32 _GainedSoul);

};
