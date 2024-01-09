// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_InvenItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_InvenItem : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* m_ItemName;
	class UTextBlock* m_ItemDesc;
	class UTextBlock* m_ItemQnt;
	class UImage* m_ItemImg;

public:
	UFUNCTION()
	void InitFromData(UObject* _Data);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

};
