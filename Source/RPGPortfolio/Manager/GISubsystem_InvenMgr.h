// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Item/Item_InvenData.h"
#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_InvenMgr.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryOpenDelegate, bool); // 인벤토리 오픈 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnClearInventoryListDelegate); // 인벤토리 아이템 클리어 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnClearEquipListDelegate); // 장비창 아이템리스트 클리어 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddInvenItemDelegate, UObject*); // 인벤토리 아이템 추가 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRenewEquipItemDelegate, EEQUIP_SLOT, UItem_InvenData*); // 인벤토리 아이템 추가 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddEquipItemListDelegate, UObject*); // 장비 아이템리스트 추가 델리게이트

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
	// UI_Inventory
	FOnInventoryOpenDelegate OnInventoryOpen;			
	FOnClearInventoryListDelegate OnClearInventoryList;
	FOnAddInvenItemDelegate OnAddInvenItem;
	// UI_EquipMain
	FOnRenewEquipItemDelegate OnRenewEquipItem;
	FOnClearEquipListDelegate OnClearEquipList;
	FOnAddEquipItemListDelegate OnAddEquipItemList;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	void SetItemDataTable(UDataTable* _ItemDataTable);
	void SetInventoryIcon(UDataAsset* _DataAsset);

	void AddGameItem(EITEM_ID _ID, uint32 _Stack);
	void SubGameItem(EEQUIP_SLOT _Slot, EITEM_ID _ID);

	void ShowInventoryUI();
	void CloseInventoryUI();

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