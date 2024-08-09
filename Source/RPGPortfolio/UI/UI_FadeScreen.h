// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_FadeScreen.generated.h"

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_FadeScreen : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UBorder* m_Screen;

	FTimerHandle FadeTimer;
	float fFadeAlpha;
	float fFadeTime;
public:
	virtual void NativeConstruct() override;

	void FadeIn(float _Time = 1.f);
	void FadeOut(float _Time = 1.f);
};
