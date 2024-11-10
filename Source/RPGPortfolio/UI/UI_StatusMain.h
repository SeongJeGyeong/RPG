// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_StatusMain.generated.h"

class UTextBlock;

/**
 * 메뉴 -> 스테이터스
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_StatusMain : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UTextBlock* m_Level;
	UTextBlock* m_Souls;
	UTextBlock* m_Required_Souls;
	UTextBlock* m_Vigor;
	UTextBlock* m_Attunement;
	UTextBlock* m_Endurance;
	UTextBlock* m_Strength;
	UTextBlock* m_Dexterity;
	UTextBlock* m_Intelligence;
	UTextBlock* m_CurrentHP;
	UTextBlock* m_MaximumHP;
	UTextBlock* m_CurrentMP;
	UTextBlock* m_MaximumMP;
	UTextBlock* m_CurrentST;
	UTextBlock* m_MaximumST;
	UTextBlock* m_PhysicAtk_Weak;
	UTextBlock* m_PhysicAtk_Strong;
	UTextBlock* m_MagicAtk;
	UTextBlock* m_PhysicDef;
	UTextBlock* m_MagicDef;

public:
	virtual void NativeConstruct() override;

	void BindEquipMgr();
	void RenewStatusUI();

};
