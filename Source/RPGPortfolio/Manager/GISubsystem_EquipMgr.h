// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_EquipMgr.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRenewStatusDelegate); // 스테이터스 갱신 델리게이트
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
	TMap<EEQUIP_SLOT, FInvenItemRow> m_EquipItemMap;

	int32 CurQuickSlotIdx = 0;

public:
	FOnRenewStatusDelegate OnRenewStatus;	// UI_StatusMain
	FOnRenewQSDelegate OnRenewQS;			// UI_Base
	FOnEmptyQSDelegate OnEmptyQS;			// UI_Base

public:
	virtual void Deinitialize() override;

public:
	FInvenItemRow* GetQSItemForIndex(int32 _Idx);

	void SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot);
	void AddStackQuickSlot(EEQUIP_SLOT _Slot, int32 _Stack);

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
