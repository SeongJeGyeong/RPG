// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_EquipMgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"
#include "../Item/Item_InvenData.h"
#include "GISubsystem_InvenMgr.h"

void UGISubsystem_EquipMgr::Deinitialize()
{
	OnRenewStatus.Clear();
	OnRenewQS.Clear();
	OnEmptyQS.Clear();

	Super::Deinitialize();
}

FInvenItemRow* UGISubsystem_EquipMgr::GetQSItemForIndex(int32 _Idx)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(ConvertIdxToQuickSlot(_Idx));
	if ( pItemRow != nullptr )
	{
		return pItemRow;
	}

	return nullptr;
}

int32 UGISubsystem_EquipMgr::GetNextIndex()
{
	return CurQuickSlotIdx % 5;
}

int32 UGISubsystem_EquipMgr::GetNextValidIndex()
{
	int32 idx = CurQuickSlotIdx;
	while ( ++idx )
	{
		idx %= 5;

		if ( GetQSItemForIndex(idx) != nullptr )
		{
			break;
		}
		if ( idx == CurQuickSlotIdx )
		{
			UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 등록된 아이템 없음"));
			break;
		}
	}
	return idx;
}

bool UGISubsystem_EquipMgr::QuickSlotValidForArr()
{
	for ( int32 i = 0; i < 5; ++i )
	{
		FInvenItemRow* Item = GetQSItemForIndex(i);
		if ( Item != nullptr )
		{
			return true;
		}
	}

	return false;
}

bool UGISubsystem_EquipMgr::QuickSlotValidForIdx(int32 _Idx)
{
	if ( GetQSItemForIndex(_Idx) != nullptr )
	{
		return true;
	}

	return false;
}

void UGISubsystem_EquipMgr::DecreaseLowerSlotItem(int32 _Idx)
{
	FInvenItemRow* pItem = GetQSItemForIndex(_Idx);

	if ( nullptr == pItem )
	{
		UE_LOG(LogTemp, Error, TEXT("DecreaseLowerSlotItem : 해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	if ( pItem->Stack > 0 )
	{
		--pItem->Stack;
		// DecreaseInventoryItem 안에 SubGameItem 포함
		UGISubsystem_InvenMgr* pInvenMgr = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
		if ( IsValid(pInvenMgr) )
		{
			pInvenMgr->DecreaseInventoryItem(pItem->ID);
		}

		// 개수가 0이 아니면 퀵슬롯에서 해당 아이템의 갯수만 줄인다.
		if ( pItem->Stack > 0 )
		{
			RenewQuickSlotUI(_Idx);
		}
	}
}

void UGISubsystem_EquipMgr::SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot)
{
	if ( _InvenItem == nullptr )
	{
		m_EquipItemMap.Remove(_Slot);
	}
	else
	{
		m_EquipItemMap.Emplace(_Slot, *_InvenItem);
	}

	OnRenewStatus.Broadcast();
}

void UGISubsystem_EquipMgr::AddStackQuickSlot(EEQUIP_SLOT _Slot, int32 _Stack)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(_Slot);
	pItemRow->Stack += _Stack;
}

UItem_InvenData* UGISubsystem_EquipMgr::GetEquipItemFromSlot(EEQUIP_SLOT _Slot)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(_Slot);
	if ( pItemRow == nullptr )
	{
		return nullptr;
	}

	UGISubsystem_InvenMgr* pInvenMgr = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
	if ( !IsValid(pInvenMgr) )
	{
		UE_LOG(LogTemp, Error, TEXT("GetEquipItemFromSlot : 인벤토리 매니저 가져오기 실패"));
		return nullptr;
	}

	UItem_InvenData* pItemData = pInvenMgr->GetInvenDataToItemRow(*pItemRow);
	return pItemData;
}

void UGISubsystem_EquipMgr::EquipQuickSlotArray(const FInvenItemRow& _InvenItem, EEQUIP_SLOT _Slot)
{
	m_EquipItemMap.Emplace(_Slot, _InvenItem);

	// 현재 퀵슬롯에 장착된 아이템이 지금 장착한 아이템 뿐일 경우
	if ( !QuickSlotValidForIdx(CurQuickSlotIdx) )
	{
		int32 Index = ConvertQuickSlotToIdx(_Slot);
		CurQuickSlotIdx = Index;
	}

	// 장착한 아이템이 어느슬롯에 장착되건 현재 보고있는 퀵슬롯 기준으로 갱신하면 알아서 표시됨
	// 퀵슬롯에 표시되는 아이템은 현재 퀵슬롯과 그 다음 슬롯중 아이템이 장착되어 있는 가장 가까운 슬롯인데, 
	// 현재 퀵슬롯을 갱신할 때 바로 다음에 아이템이 장착되어 있는 슬롯을 찾아서 갱신하기 때문
	RenewQuickSlotUI(CurQuickSlotIdx);
}

void UGISubsystem_EquipMgr::UnEquipQuickSlotArray(EEQUIP_SLOT _Slot)
{
	m_EquipItemMap.Remove(_Slot);
	if ( ConvertQuickSlotToIdx(_Slot) == CurQuickSlotIdx )
	{
		// 현재 퀵슬롯의 아이템을 장비해제 했을경우 다음 장착아이템이 있는 퀵슬롯으로 자리를 옮긴다.
		CurQuickSlotIdx = GetNextValidIndex();
	}

	if ( QuickSlotValidForIdx(CurQuickSlotIdx) )
	{
		// 현재 퀵슬롯 ui 갱신
		RenewQuickSlotUI(CurQuickSlotIdx);
	}
	else
	{
		// 다른 슬롯에 장비된 아이템이 없을 경우 퀵슬롯 UI 비우기
		EmptyQuickSlotUI();
	}
}

void UGISubsystem_EquipMgr::RenewQuickSlotUI(int32 _Idx)
{
	OnRenewQS.Broadcast(_Idx);
}

void UGISubsystem_EquipMgr::EmptyQuickSlotUI()
{
	OnEmptyQS.Broadcast();
}

int32 UGISubsystem_EquipMgr::ConvertQuickSlotToIdx(EEQUIP_SLOT _Slot)
{
	int32 Index = -1;
	switch ( _Slot )
	{
	case EEQUIP_SLOT::CONSUMABLE_1:
		Index = 0;
		break;
	case EEQUIP_SLOT::CONSUMABLE_2:
		Index = 1;
		break;
	case EEQUIP_SLOT::CONSUMABLE_3:
		Index = 2;
		break;
	case EEQUIP_SLOT::CONSUMABLE_4:
		Index = 3;
		break;
	case EEQUIP_SLOT::CONSUMABLE_5:
		Index = 4;
		break;
	case EEQUIP_SLOT::EMPTY:
		Index = -1;
		break;
	default:
		break;
	}

	return Index;
}

EEQUIP_SLOT UGISubsystem_EquipMgr::ConvertIdxToQuickSlot(int32 _Idx)
{
	EEQUIP_SLOT Slot = EEQUIP_SLOT::EMPTY;
	switch ( _Idx )
	{
	case 0:
		Slot = EEQUIP_SLOT::CONSUMABLE_1;
		break;
	case 1:
		Slot = EEQUIP_SLOT::CONSUMABLE_2;
		break;
	case 2:
		Slot = EEQUIP_SLOT::CONSUMABLE_3;
		break;
	case 3:
		Slot = EEQUIP_SLOT::CONSUMABLE_4;
		break;
	case 4:
		Slot = EEQUIP_SLOT::CONSUMABLE_5;
		break;
	default:
		break;
	}

	return Slot;
}
