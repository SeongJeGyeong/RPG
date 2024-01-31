// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_StatusMain.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_StatusMain : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* m_Level;
	class UTextBlock* m_Souls;
	class UTextBlock* m_Required_Souls;
	class UTextBlock* m_Vigor;
	class UTextBlock* m_Attunement;
	class UTextBlock* m_Endurance;
	class UTextBlock* m_Strength;
	class UTextBlock* m_Dexterity;
	class UTextBlock* m_Intelligence;
	class UTextBlock* m_CurrentHP;
	class UTextBlock* m_MaximumHP;
	class UTextBlock* m_CurrentMP;
	class UTextBlock* m_MaximumMP;
	class UTextBlock* m_CurrentST;
	class UTextBlock* m_MaximumST;
	class UTextBlock* m_PhysicAtk_Weak;
	class UTextBlock* m_PhysicAtk_Strong;
	class UTextBlock* m_MagicAtk;
	class UTextBlock* m_PhysicDef;
	class UTextBlock* m_MagicDef;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

};
