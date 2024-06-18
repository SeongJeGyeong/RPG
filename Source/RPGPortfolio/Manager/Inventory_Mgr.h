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
	UPROPERTY()
	UDataTable* m_ItemDataTable;
	UPROPERTY()
	TMap<EITEM_ID, FGameItemInfo>	m_MapItemInfo;

	TMap<EITEM_ID, FInvenItemRow> m_InvenStorage[ (int32)EITEM_TYPE::END ];

public:
	// 스태틱 함수로 선언하여 객체가 생성되지 않았어도 함수 호출 가능
	static UInventory_Mgr* GetInst(UWorld* _World);
	static UInventory_Mgr* GetInst(UGameInstance* _GameInst);

	void SetItemDataTable(UDataTable* _ItemDataTable);
	void AddGameItem(EITEM_ID _ID, uint32 _Stack);
	void SubGameItem(EEQUIP_SLOT _Slot, EITEM_ID _ID);

	void ShowInventoryUI();
	void CloseInventoryUI();
	bool CheckInventoryOpened();

	void ChangeEquipItem(EITEM_ID _ID, EEQUIP_SLOT _Slot);

	FGameItemInfo* GetItemInfo(EITEM_ID _ID);
public:
	void RenewInventoryUI(EITEM_TYPE _Type);
	void RenewItemListUI(EITEM_TYPE _Type);
	void RenewEquipConsumeUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow, bool _Unequip);
	void RenewEquipItemUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow);
	void DecreaseInventoryItem(EITEM_ID _ID);
};
