// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory_Mgr.generated.h"

class UPaperSprite;

/*
 *  사용안함
 */
UCLASS()
class RPGPORTFOLIO_API UInventory_Mgr : public UObject
{
	GENERATED_BODY()
	
private:
	static UWorld* m_World;

private:
	UPROPERTY()
	UDataTable* m_ItemDataTable;
	UPROPERTY()
	TMap<EITEM_ID, FGameItemInfo>	m_MapItemInfo;	// 게임의 모든 아이템 정보가 담긴 맵
	UPROPERTY()
	TArray<FInvenItemMap> m_InvenStorage;	// 플레이어가 인벤토리에 소지 중인 아이템 맵

	UPROPERTY()
	class UDA_ItemCategoryIcon* m_Icon;

public:
	// 스태틱 함수로 선언하여 객체가 생성되지 않았어도 함수 호출 가능
	//static UInventory_Mgr* GetInst(UWorld* _World);
	//static UInventory_Mgr* GetInst(UGameInstance* _GameInst);

	void SetItemDataTable(UDataTable* _ItemDataTable);
	void SetInventoryIcon(UDataAsset* _DataAsset);

	void AddGameItem(EITEM_ID _ID, uint32 _Stack);
	void SubGameItem(EEQUIP_SLOT _Slot, EITEM_ID _ID);

	void ShowInventoryUI();
	void CloseInventoryUI();
	bool CheckInventoryOpened();

	void ChangeEquipItem(EITEM_ID _ID, EEQUIP_SLOT _Slot);

	FGameItemInfo* GetItemInfo(EITEM_ID _ID);
	FInvenItemRow* GetInvenItemInfo(EITEM_ID _ID);
public:
	void RenewInventoryUI(EITEM_TYPE _Type);
	void RenewEquipItemListUI(EITEM_TYPE _Type);
	void EquipConsumeUI(EEQUIP_SLOT _Slot, const FInvenItemRow& _ItemRow);
	void UnEquipConsumeUI(EEQUIP_SLOT _Slot);
	void RenewEquipItemUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow);
	void DecreaseInventoryItem(EITEM_ID _ID);

	UPaperSprite* GetCategoryIcon(EITEM_TYPE _type);
	UPaperSprite* GetEquipSlotIcon(EEQUIP_SLOT _Slot);
	class UItem_InvenData* GetInvenDataToItemRow(const FInvenItemRow& _ItemRow);
};
