// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Settings.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Settings : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	class UButton*		m_Btn_Quit;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	class UTextBlock* m_PlayerName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	class UTextBlock* m_Level;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	class UTextBlock* m_Playtime;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true"))
	class UTextBlock* m_MapName;

	UPROPERTY()
	class UDA_MenuSound* m_Sound;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

public:
	UFUNCTION()
	void SettingsVisibilityChanged(ESlateVisibility _Visibility);

	UFUNCTION()
	void QuitBtnClicked();
	UFUNCTION()
	void QuitBtnHovered();
	UFUNCTION()
	void QuitBtnUnHovered();
};
