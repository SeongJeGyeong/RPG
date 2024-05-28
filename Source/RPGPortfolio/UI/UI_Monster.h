// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Monster.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Monster : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class UTextBlock* m_Name;
	UPROPERTY()
	class UTextBlock* m_DMGFigure;
	UPROPERTY()
	class UProgressBar* m_HPBar;

	FText m_MonsterName;
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

	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

};
