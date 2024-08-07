// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Player_Soul.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Player_Soul : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock* m_Soul;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock* m_GainSoul;

	UPROPERTY()
	class UDA_MenuSound* m_Sound;
	
	int32 iDisplayedSoul;
	int32 iGainedSoul;

	float fOpacity = 0.f;

	FTimerHandle SoulGainTimer;
	FTimerHandle RenewSoulTimer;
	FTimerHandle FadeOutSoulTimer;

public:
	virtual void NativeConstruct() override;

	void RenewAmountOfSoul(int32 _GainedSoul);
	void StartSoulGain();
	void SoulGain();
	void FadeOutSoul();
};
