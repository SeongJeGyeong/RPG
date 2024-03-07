// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ItemSelectMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UUI_ItemSelectMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	class UTextBlock* m_Txt_Use;
	class UButton* m_Btn_Use;
	class UButton* m_Btn_Drop;
	class UButton* m_Btn_Discard;
	class UButton* m_Btn_DropAll;
	class UButton* m_Btn_DiscardAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	class UItem_InvenData* m_SelectedItemData;


public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

public:
	UFUNCTION()
	void SetItemData(UItem_InvenData* _SelectedItemData) { m_SelectedItemData = _SelectedItemData; }

	UFUNCTION()
	void UseBtnClicked();

};
