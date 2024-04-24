// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Equip_Mgr.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UEquip_Mgr : public UObject
{
	GENERATED_BODY()

private:
	static UWorld* m_World;

	UPROPERTY()
	TMap<EEQUIP_SLOT, FGameItemInfo> m_EquipItemMap;

	// TFixedAllocator : 지정한 숫자만큼의 배열 메모리를 정적으로 할당
	// 할당된 인덱스를 넘어서 엘리먼트를 추가하려고 하면 코드 오류 발생
	TArray<FInvenItemRow*, TFixedAllocator<5>> m_QuickSlotArr = {nullptr, nullptr, nullptr, nullptr, nullptr};

	int32 CurQuickSlotIdx = 0;

public:
	static UEquip_Mgr* GetInst(UWorld* _World);
	static UEquip_Mgr* GetInst(UGameInstance* _GameInst);

	FInvenItemRow* GetQSItemForIndex(int32 _Idx);

	void SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot);
	FGameItemInfo* GetEquipItemFromSlot(EEQUIP_SLOT _Slot) { return m_EquipItemMap.Find(_Slot); }

	void SetQuickSlotArray(FInvenItemRow* _InvenItem, int32 _Idx, bool _Unequip);
	void RenewQuickSlotUI(int32 _Idx);
	void RenewNextQuickSlotUI(int32 _Idx);

	int32 ConvertQuickSlotToIdx(EEQUIP_SLOT _Slot);
	EEQUIP_SLOT ConvertIdxToQuickSlot(int32 _Idx);

	int32 GetCurrentIndex() { return CurQuickSlotIdx; }
	int32 GetNextArrayIndex();
	void SetCurrentIndex(int32 _CurrentIdx) { CurQuickSlotIdx = _CurrentIdx; }
	bool QuickSlotValidForArr();
	bool QuickSlotValidForIdx(int32 _Idx);

	void DecreaseLowerSlotItem(int32 _Idx);
};
