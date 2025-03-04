// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Monster.generated.h"

class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Monster : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock* m_MonsterName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock* m_MonDMGFigure;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UProgressBar* m_MonsterHP;

	FText m_Name;
	float m_Ratio;

	float fDisplayTime = 0.f;

	bool  bDMGDisplay;
	float fTakedDMG = 0.f;
	
	FTimerHandle DmgDisplayTimer;
public:
	void SetHPRatio(float _Ratio);
	void SetName(const FString& _Name);
	void DisplayDMG(const float _DMG);

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
