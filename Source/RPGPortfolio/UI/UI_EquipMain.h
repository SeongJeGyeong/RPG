// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipMain.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_EquipMain : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UUI_ItemTooltip*	m_Tooltip;
	class UUI_PlayerStat*	m_Stat;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;


};
