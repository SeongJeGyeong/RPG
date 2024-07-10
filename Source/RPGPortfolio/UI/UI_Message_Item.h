// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Message_Item.generated.h"

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_Message_Item : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UImage*		m_ItemImg;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock*	m_ItemName;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock*	m_ItemQnt;

public:
	virtual void NativeConstruct() override;

	void SetItemMessage(FString _ItemName, FString _ItemImgPath, int32 _Stack);
	void SetItemMessage(FString _ItemName, UTexture2D* _Texture, int32 _Stack);
};
