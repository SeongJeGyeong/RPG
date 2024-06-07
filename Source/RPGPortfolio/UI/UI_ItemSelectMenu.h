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
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UTextBlock* m_Txt_Use;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UButton* m_Btn_Use;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UButton* m_Btn_Drop;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UButton* m_Btn_Discard;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UButton* m_Btn_DropAll;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	class UButton* m_Btn_DiscardAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	class UItem_InvenData* m_SelectedItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	bool bItemUseDelay;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _Geo, float _DeltaTime) override;

public:
	void SetSelectedItemData(class UItem_InvenData* _ItemData) { m_SelectedItemData = _ItemData; }
	void SetbItemUseDelay(const bool& _ItemUseDelay) { bItemUseDelay = _ItemUseDelay; }

	UFUNCTION()
	void UseBtnClicked();

};
