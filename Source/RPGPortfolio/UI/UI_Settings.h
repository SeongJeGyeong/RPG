// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Settings.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Settings : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UButton*	m_Btn_Quit;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UButton*	m_Btn_GSettings;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UTextBlock* m_PlayerName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UTextBlock* m_Level;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UTextBlock* m_Playtime;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true"))
	UTextBlock* m_MapName;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UUI_Settings_GPanel* m_GSettingsPannel;

public:
	virtual void NativeConstruct() override;

public:
	bool GetGameSettingPannelVisibility();
	void CloseGameSettingPannel();

	UFUNCTION()
	void SettingsVisibilityChanged(ESlateVisibility _Visibility);

	UFUNCTION()
	void QuitBtnClicked();
	UFUNCTION()
	void QuitBtnHovered();

	UFUNCTION()
	void GSettingBtnClicked();
	UFUNCTION()
	void GSettingBtnHovered();
};
