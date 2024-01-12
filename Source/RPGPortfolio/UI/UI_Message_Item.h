// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Message_Item.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_Message_Item : public UUserWidget
{
	GENERATED_BODY()

private:
	class UImage*		m_ItemImg;
	class UTextBlock*	m_ItemName;
	class UTextBlock* m_ItemQnt;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

};
