// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Player_Main.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Player_Main : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UProgressBar* m_PlayerHP;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UProgressBar* m_PlayerMP;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UProgressBar* m_PlayerStamina;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void SetPlayerHPRatio(float _HPRatio);
	void SetPlayerMPRatio(float _MPRatio);
	void SetPlayerSTRatio(float _STRatio);
};
