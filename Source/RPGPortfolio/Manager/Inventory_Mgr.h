// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory_Mgr.generated.h"

UCLASS()
class RPGPORTFOLIO_API UInventory_Mgr : public UObject
{
	GENERATED_BODY()
	
private:
	static UWorld* m_World;

private:
	UDataTable* m_ItemDataTable;
	TMap<EITEM_ID, FGameItemInfo>	m_MapItemInfo;
	TMap<EITEM_ID, FInvenItemRow>	m_InvenStorage[(int32)EITEM_TYPE::END];

public:
	static UInventory_Mgr* GetInst(UWorld* _World);
	static UInventory_Mgr* GetInst(UGameInstance* _GameInst);

	void SetItemDataTable(UDataTable* _ItemDataTable);
	void AddGameItem(EITEM_ID _ID);
	void SubGameItem(EITEM_ID _ID);

	void ShowInventoryUI();
	void CloseInventoryUI();
	bool CheckInventoryOpened();

	void ChangeEquipItem(EITEM_ID _ID, EEQUIP_SLOT _Slot);

public:
	void RenewInventoryUI(EITEM_TYPE _Type);
	void RenewItemListUI(EITEM_TYPE _Type);
	void RenewEquipConsumeUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow, bool _Unequip);
	void RenewEquipItemUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow);
};
