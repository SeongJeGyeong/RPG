// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_GameSettingBox.generated.h"

UENUM(BlueprintType)
enum class ESETTING_PROPERTY : uint8
{
	SCREEN_MODE,
	RESOLUTION,
	VIEW_DISTANCE,
	ANTI_ALIASING,
	POST_PROCESSING,
	SHADOW,
	GLOBAL_ILLUMINATION,
	REFLECTION,
	TEXTURE,
	EFFECT,
	FOLIAGE,
	SHADING,
	VSYNC,
	SOUND,
};

class UButton;
class UTextBlock;
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_GameSettingBox : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = "true" ))
	ESETTING_PROPERTY Setting_Property;

	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock* Txt_SettingProperty;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UTextBlock* Txt_SettingLevel;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* Btn_Down;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* Btn_Up;

public:
	virtual void NativeConstruct() override;

	void OnVisibilityChanged(ESlateVisibility _Visibility);

	void SetUserSettingValText(int32 _Val);
	void SetUserSettingValText(float _Val);
	void SetUserSettingValText(bool _Val);
	void SetUserSettingValText(FIntPoint _Val);
	void SetUserSettingValText(EWindowMode::Type _Val);

	EWindowMode::Type ChangeScreenMode(EWindowMode::Type _Type, bool _UpDown);
	FIntPoint ChangeScreenResolution(FIntPoint _Res, bool _UpDown);

	UFUNCTION()
	void DownBtnClicked();
	UFUNCTION()
	void DownBtnHovered();

	UFUNCTION()
	void UpBtnClicked();
	UFUNCTION()
	void UpBtnHovered();
};
