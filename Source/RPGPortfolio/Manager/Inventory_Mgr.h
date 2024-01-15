// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory_Mgr.generated.h"

USTRUCT()
struct FInvenItemRow
{
	GENERATED_USTRUCT_BODY()

	FGameItemInfo*	ItemInfo;
	uint32		Stack;
};

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
	TSubclassOf<UUserWidget> InvenWidgetClass;
	class UUI_Inventory* InvenUI;

public:
	static UInventory_Mgr* GetInst(UWorld* _World);
	static UInventory_Mgr* GetInst(UGameInstance* _GameInst);

	void SetItemDataTable(UDataTable* _ItemDataTable);
	void AddGameItem(EITEM_ID _ID);

	void ShowInventoryUI();
	void CloseInventoryUI();

	bool IsInventoryOpened();

private:
	void RenewInventoryUI();

public:
	UInventory_Mgr();

};
