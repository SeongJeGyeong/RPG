// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipItem.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_EquipItem : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UButton*	m_ItemBtn;
	class UImage*	m_ItemImg;
	class UImage*	m_DishImg;

	class UItem_InvenData* m_ItemData;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;
};
