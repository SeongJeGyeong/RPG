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
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock* m_Soul;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock* m_GainSoul;

	UPROPERTY()
	class UDA_MenuSound* m_Sound;
	
	int32 iDisplayedSoul;
	int32 iGainedSoul;
	bool bSoulGained = false;
	bool bFadeOutGainedSoul = false;

	float fOpacity = 0.f;

	FTimerHandle SoulGainTimer;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void RenewAmountOfSoul(int32 _GainedSoul);
	void StartSoulGain();
};
