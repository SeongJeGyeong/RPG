// Fill out your copyright notice in the Description page of Project Settings.


#include "Equip_Mgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"
#include "../UI/UI_Player_QuickSlot.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_StatusMain.h"
#include "../UI/UI_EquipMain.h"
//#include "Inventory_Mgr.h"
#include "../Item/Item_InvenData.h"

UWorld* UEquip_Mgr::m_World = nullptr;

//UEquip_Mgr* UEquip_Mgr::GetInst(UWorld* _World)
//{
//    m_World = _World;
//	
//    return GetInst(m_World->GetGameInstance());
//}
//
//UEquip_Mgr* UEquip_Mgr::GetInst(UGameInstance* _GameInst)
//{
//	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(_GameInst);
//
//	if (!IsValid(pGameInst->m_EquipMgr))
//	{
//		pGameInst->m_EquipMgr = NewObject<UEquip_Mgr>();
//		pGameInst->m_EquipMgr->AddToRoot();
//	}
//
//	return pGameInst->m_EquipMgr;
//}

FInvenItemRow* UEquip_Mgr::GetQSItemForIndex(int32 _Idx)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(ConvertIdxToQuickSlot(_Idx));
	if (pItemRow != nullptr)
	{
		return pItemRow;
	}

	return nullptr;
}

int32 UEquip_Mgr::GetNextIndex()
{
	return CurQuickSlotIdx % 5;
}

int32 UEquip_Mgr::GetNextValidIndex()
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

bool UEquip_Mgr::QuickSlotValidForArr()
{
	for (int32 i = 0; i < 5; ++i)
	{
		FInvenItemRow* Item = GetQSItemForIndex(i);
		if (Item != nullptr)
		{
			return true;
		}
	}

	return false;
}

bool UEquip_Mgr::QuickSlotValidForIdx(int32 _Idx)
{
	if ( GetQSItemForIndex(_Idx) != nullptr)
	{
		return true;
	}

	return false;
}

void UEquip_Mgr::DecreaseLowerSlotItem(int32 _Idx)
{
	FInvenItemRow* pItem = GetQSItemForIndex(_Idx);

	if (nullptr == pItem)
	{
		UE_LOG(LogTemp, Error, TEXT("DecreaseLowerSlotItem : 해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	if (pItem->Stack > 0)
	{
		--pItem->Stack;
		// DecreaseInventoryItem 안에 SubGameItem 포함
		//UInventory_Mgr::GetInst(m_World)->DecreaseInventoryItem(pItem->ID);

		// 개수가 0이 아니면 퀵슬롯에서 해당 아이템의 갯수만 줄인다.
		if( pItem->Stack > 0 )
		{
			RenewQuickSlotUI(_Idx);
		}
	}
}

void UEquip_Mgr::SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		return;
	}

	if ( _InvenItem == nullptr)
	{
		m_EquipItemMap.Remove(_Slot);
	}
	else
	{
		m_EquipItemMap.Emplace(_Slot, *_InvenItem);
	}

	UUI_StatusMain* StatusUI = GameMode->GetStatusUI();
	StatusUI->RenewStatusUI();
}

UItem_InvenData* UEquip_Mgr::GetEquipItemFromSlot(EEQUIP_SLOT _Slot)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(_Slot);
	if (pItemRow == nullptr)
	{
		return nullptr;
	}

	//UItem_InvenData* pItemData = UInventory_Mgr::GetInst(m_World)->GetInvenDataToItemRow(*pItemRow);
	//return pItemData;
	return nullptr;
}

void UEquip_Mgr::EquipQuickSlotArray(const FInvenItemRow& _InvenItem, EEQUIP_SLOT _Slot)
{
	m_EquipItemMap.Emplace(_Slot, _InvenItem);
	
	// 현재 퀵슬롯에 장착된 아이템이 지금 장착한 아이템 뿐일 경우
	if (!QuickSlotValidForIdx(CurQuickSlotIdx))
	{
		int32 Index = ConvertQuickSlotToIdx(_Slot);
		CurQuickSlotIdx = Index;
	}

	// 장착한 아이템이 어느슬롯에 장착되건 현재 보고있는 퀵슬롯 기준으로 갱신하면 알아서 표시됨
	// 퀵슬롯에 표시되는 아이템은 현재 퀵슬롯과 그 다음 슬롯중 아이템이 장착되어 있는 가장 가까운 슬롯인데, 
	// 현재 퀵슬롯을 갱신할 때 바로 다음에 아이템이 장착되어 있는 슬롯을 찾아서 갱신하기 때문
	RenewQuickSlotUI(CurQuickSlotIdx);
}

void UEquip_Mgr::UnEquipQuickSlotArray(EEQUIP_SLOT _Slot)
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

void UEquip_Mgr::RenewQuickSlotUI(int32 _Idx)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("RenewQuickSlotUI 게임모드 캐스팅 실패"));
		return;
	}
	UUI_Base* MainUI = GameMode->GetMainHUD();

	MainUI->GetQuickSlotUI()->RenewLowerQuickSlot(_Idx);
}

void UEquip_Mgr::EmptyQuickSlotUI()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("EmptyQuickSlotUI 게임모드 캐스팅 실패"));
		return;
	}
	UUI_Base* MainUI = GameMode->GetMainHUD();

	MainUI->GetQuickSlotUI()->EmptyLowerQuickSlot();
}

int32 UEquip_Mgr::ConvertQuickSlotToIdx(EEQUIP_SLOT _Slot)
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

EEQUIP_SLOT UEquip_Mgr::ConvertIdxToQuickSlot(int32 _Idx)
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
