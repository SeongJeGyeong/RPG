// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Item/Item_InvenData.h"
#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_InvenMgr.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClearInventoryListDelegate);										// 인벤토리 아이템 클리어 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnClearEquipListDelegate);											// 장비창 아이템리스트 클리어 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddInvenItemDelegate, UObject*);							// 인벤토리 아이템 추가 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRenewEquipItemDelegate, EEQUIP_SLOT, UItem_InvenData*); // 장비창 장착된 아이템 갱신 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddEquipItemListDelegate, UObject*);						// 장비 아이템리스트 추가 델리게이트

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRenewQSDelegate, UItem_InvenData*, UItem_InvenData*); // 퀵슬롯 갱신 델리게이트

class UPaperSprite;
class UItem_InvenData;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_InvenMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TMap<EITEM_ID, FGameItemInfo>	m_MapItemInfo;		// 게임의 모든 아이템 정보가 담긴 맵
	UPROPERTY()
	TArray<FInvenItemMap> m_InvenStorage;				// 플레이어가 인벤토리에 소지 중인 아이템 맵
	UPROPERTY()
	TMap<EEQUIP_SLOT, FInvenItemRow> m_EquipItemMap;			// 플레이어가 장비중인 아이템 맵

	int32 CurQuickSlotIdx = 0;

public:
	// UI_Inventory
	FOnClearInventoryListDelegate OnClearInventoryList;
	FOnAddInvenItemDelegate OnAddInvenItem;
	// UI_EquipMain
	FOnRenewEquipItemDelegate OnRenewEquipItem;
	FOnClearEquipListDelegate OnClearEquipList;
	FOnAddEquipItemListDelegate OnAddEquipItemList;
	// UI_Base
	FOnRenewQSDelegate OnRenewQS;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	void SetItemDataTable(UDataTable* _ItemDataTable);
	void EquipConsumeUI(EEQUIP_SLOT _Slot, const FInvenItemRow& _ItemRow);
	void UnEquipConsumeUI(EEQUIP_SLOT _Slot);
	void RenewEquipItemUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow);

	void SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot);

public:
	FInvenItemRow* AddGameItem(EITEM_ID _ID, uint32 _Stack);
	void SubGameItem(EEQUIP_SLOT _Slot, EITEM_ID _ID);

	void ChangeEquipItem(EITEM_ID _ID, EEQUIP_SLOT _Slot);

	FGameItemInfo* GetItemInfo(EITEM_ID _ID);
	FInvenItemRow* GetInvenItemInfo(EITEM_ID _ID);
	UItem_InvenData* GetEquipItemFromSlot(EEQUIP_SLOT _Slot);

	void RenewInventoryUI(EITEM_TYPE _Type);
	void RenewEquipItemListUI(EITEM_TYPE _Type);
	void DecreaseInventoryItem(EITEM_ID _ID);
	void DecreaseLowerSlotItem(EEQUIP_SLOT _Slot);
	void IncreaseEquipItemStack(EEQUIP_SLOT _Slot, int32 _Stack);

	void RenewQuickSlotUI();

	class UItem_InvenData* GetInvenDataToItemRow(const FInvenItemRow& _ItemRow);

	FInvenItemRow* GetQSItemForIndex(int32 _Idx);
	FInvenItemRow* GetCurrentQSItem();

	EITEM_TYPE GetItemTypeFromSlot(EEQUIP_SLOT _Slot);

	int32 GetCurrentIndex() const { return CurQuickSlotIdx; }
	int32 GetNextValidIndex();

	void SetCurrentIndex(const int32& _CurrentIdx) { CurQuickSlotIdx = _CurrentIdx; }
	int32 ConvertQuickSlotToIdx(EEQUIP_SLOT _Slot);
	EEQUIP_SLOT ConvertIdxToQuickSlot(int32 _Idx);
};