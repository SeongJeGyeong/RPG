// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_EquipItem.generated.h"

class UButton;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS(Meta = ( DisableNativeTick ))
class RPGPORTFOLIO_API UUI_EquipItem : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable", meta = ( AllowPrivateAccess = true ))
	EEQUIP_SLOT	eSlotType;

	// UPROPERTY 리플렉션을 붙여주어야 가비지 컬렉션에서 관리되며, 해당 멤버가 포함된 클래스와 동일한 수명주기를 갖게 된다.
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UButton* m_EquipItemBtn;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage* m_EquipItemImg;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage* m_EquipDishImg;
	UPROPERTY(BlueprintReadWrite, meta = ( BindWidgetOptional, AllowPrivateAccess = "true" ))
	UImage* m_BackSlotImg;

	// 장비창 메인 화면의 위젯들. 블루프린트에서 참조받는다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	UTextBlock* ItemSlotText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	UTextBlock* ItemNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UUI_EquipItemList*	ItemList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = ( AllowPrivateAccess = true ))
	class UUI_ItemTooltip* m_Tooltip;

public:
	virtual void NativeConstruct() override;

	void SetSlotType(EEQUIP_SLOT _Type) { eSlotType = _Type; }
	EEQUIP_SLOT GetSlotType() { return eSlotType; }
	UButton* GetItemBtn() { return m_EquipItemBtn; }

	void SetEquipItem(class UItem_InvenData* _ItemData);

	UFUNCTION()
	void ItemBtnClicked();
	UFUNCTION()
	void ItemBtnHovered();
	UFUNCTION()
	void ItemBtnUnHovered();
};
