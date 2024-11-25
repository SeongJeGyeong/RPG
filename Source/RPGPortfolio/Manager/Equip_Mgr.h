// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Equip_Mgr.generated.h"

/**
 *	사용안함
 */
UCLASS()
class RPGPORTFOLIO_API UEquip_Mgr : public UObject
{
	GENERATED_BODY()

private:
	static UWorld* m_World;

	UPROPERTY()
	TMap<EEQUIP_SLOT, FInvenItemRow> m_EquipItemMap;

	int32 CurQuickSlotIdx = 0;

public:
	//static UEquip_Mgr* GetInst(UWorld* _World);
	//static UEquip_Mgr* GetInst(UGameInstance* _GameInst);

	FInvenItemRow* GetQSItemForIndex(int32 _Idx);

	void SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot);

	class UItem_InvenData* GetEquipItemFromSlot(EEQUIP_SLOT _Slot);

	void EquipQuickSlotArray(const FInvenItemRow& _InvenItem, EEQUIP_SLOT _Slot);
	void UnEquipQuickSlotArray(EEQUIP_SLOT _Slot);
	void RenewQuickSlotUI(int32 _Idx);
	void EmptyQuickSlotUI();
	int32 ConvertQuickSlotToIdx(EEQUIP_SLOT _Slot);
	EEQUIP_SLOT ConvertIdxToQuickSlot(int32 _Idx);

	int32 GetCurrentIndex() { return CurQuickSlotIdx; }
	int32 GetNextIndex();
	int32 GetNextValidIndex();
	void SetCurrentIndex(int32 _CurrentIdx) { CurQuickSlotIdx = _CurrentIdx; }
	bool QuickSlotValidForArr();
	bool QuickSlotValidForIdx(int32 _Idx);

	void DecreaseLowerSlotItem(int32 _Idx);
};
