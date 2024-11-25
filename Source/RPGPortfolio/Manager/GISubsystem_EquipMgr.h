// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_EquipMgr.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRenewQSDelegate, int32); // 퀵슬롯 갱신 델리게이트
DECLARE_MULTICAST_DELEGATE(FOnEmptyQSDelegate); // 퀵슬롯 비우기 델리게이트
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_EquipMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TMap<EEQUIP_SLOT, FInvenItemRow> m_QSItemMap;

	int32 CurQuickSlotIdx = 0;

public:
	FOnRenewQSDelegate OnRenewQS;			// UI_Base
	FOnEmptyQSDelegate OnEmptyQS;			// UI_Base

public:
	virtual void Deinitialize() override;

public:
	void AddStackQuickSlot(EEQUIP_SLOT _Slot, int32 _Stack);
	FInvenItemRow* GetQSItemForIndex(int32 _Idx);

	void EquipQuickSlotArray(const FInvenItemRow& _InvenItem, EEQUIP_SLOT _Slot);
	void UnEquipQuickSlotArray(EEQUIP_SLOT _Slot);
	void RenewQuickSlotUI(int32 _Idx);
	void EmptyQuickSlotUI();
	void DecreaseLowerSlotItem(int32 _Idx);

	int32 ConvertQuickSlotToIdx(EEQUIP_SLOT _Slot);
	EEQUIP_SLOT ConvertIdxToQuickSlot(int32 _Idx);

	int32 GetCurrentIndex() { return CurQuickSlotIdx; }
	int32 GetNextIndex();
	int32 GetNextValidIndex();
	void SetCurrentIndex(int32 _CurrentIdx) { CurQuickSlotIdx = _CurrentIdx; }
	bool QuickSlotValidForArr();
	bool QuickSlotValidForIdx(int32 _Idx);
};
