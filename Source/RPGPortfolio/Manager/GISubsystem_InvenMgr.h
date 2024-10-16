// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_InvenMgr.generated.h"

class UPaperSprite;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_InvenMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
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
	void RenewItemListUI(EITEM_TYPE _Type);
	void EquipConsumeUI(EEQUIP_SLOT _Slot, const FInvenItemRow& _ItemRow);
	void UnEquipConsumeUI(EEQUIP_SLOT _Slot);
	void RenewEquipItemUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow);
	void DecreaseInventoryItem(EITEM_ID _ID);

	UPaperSprite* GetCategoryIcon(EITEM_TYPE _type);
	UPaperSprite* GetEquipSlotIcon(EEQUIP_SLOT _Slot);
	class UItem_InvenData* GetInvenDataToItemRow(const FInvenItemRow& _ItemRow);

};
