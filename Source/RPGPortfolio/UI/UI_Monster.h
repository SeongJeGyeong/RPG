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
	class UTextBlock* m_Name;
	class UTextBlock* m_DMGFigure;
	class UProgressBar* m_HPBar;

	FText m_MonsterName;
	float m_Ratio;

public:
	void SetHPRatio(float _Ratio);
	void SetName(const FString& _Name);
	void DisplayDMG(const int32 _DMG);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

};
