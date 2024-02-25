// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ItemTooltip.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_ItemTooltip : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* m_ItemName;
	class UTextBlock* m_Category;
	class UTextBlock* m_SubCategory;
	class UTextBlock* m_CurQnt_Inven;
	class UTextBlock* m_MaxQnt_Inven;
	class UTextBlock* m_CurQnt_Storage;
	class UTextBlock* m_MaxQnt_Storage;
	class UTextBlock* m_PhysicAtk;
	class UTextBlock* m_PhysicDef;
	class UTextBlock* m_MagicAtk;
	class UTextBlock* m_MagicDef;
	class UTextBlock* m_Restore_HP;
	class UTextBlock* m_Restore_MP;
	class UTextBlock* m_Desc;
	class UTextBlock* m_Req_Str;
	class UTextBlock* m_Req_Dex;
	class UTextBlock* m_Req_Int;

	class UImage* m_ItemImg;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

	void SetTooltipUI(class UItem_InvenData* _InvenData);
};
