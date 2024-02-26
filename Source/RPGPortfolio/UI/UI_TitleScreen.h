// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_TitleScreen.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_TitleScreen : public UUserWidget
{
	GENERATED_BODY()

private:
	class UButton* m_NewBtn;
	class UButton* m_LoadBtn;
	class UButton* m_QuitBtn;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

public:
	UFUNCTION()
	void NewBtnClicked();
	UFUNCTION()
	void NewBtnHovered();
	UFUNCTION()
	void NewBtnUnHovered();

	UFUNCTION()
	void LoadBtnClicked();
	UFUNCTION()
	void LoadBtnHovered();
	UFUNCTION()
	void LoadBtnUnHovered();

	UFUNCTION()
	void QuitBtnClicked();
	UFUNCTION()
	void QuitBtnHovered();
	UFUNCTION()
	void QuitBtnUnHovered();
};