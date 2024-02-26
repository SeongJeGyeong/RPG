// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayerStat.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_PlayerStat : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* m_Level;
	class UTextBlock* m_Vigor;
	class UTextBlock* m_Attunement;
	class UTextBlock* m_Endurance;
	class UTextBlock* m_Strength;
	class UTextBlock* m_Dexterity;
	class UTextBlock* m_Intelligence;

	class UTextBlock* m_MaxHP;
	class UTextBlock* m_MaxMP;
	class UTextBlock* m_MaxStamina;
	class UTextBlock* m_CurHP;
	class UTextBlock* m_CurMP;
	class UTextBlock* m_CurStamina;

	class UTextBlock* m_PhysicAtk;
	class UTextBlock* m_PhysicDef;
	class UTextBlock* m_MagicAtk;
	class UTextBlock* m_MagicDef;
	class UTextBlock* m_AltPhysicAtk;
	class UTextBlock* m_AltPhysicDef;
	class UTextBlock* m_AltMagicAtk;
	class UTextBlock* m_AltMagicDef;
	class UTextBlock* m_PhyAtk_Arrow;
	class UTextBlock* m_PhyDef_Arrow;
	class UTextBlock* m_MagAtk_Arrow;
	class UTextBlock* m_MagDef_Arrow;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void SetPlayerStatUI(class APlayerState_Base* _PlayerState);
	void RenewBasePower();
	void SetVisibilityAlterBasePower(bool _bVisibility);
	void AlterRenewBasePower(UItem_InvenData* _InvenData, bool _bEquiped);
};
