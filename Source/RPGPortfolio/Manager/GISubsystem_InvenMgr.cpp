// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_InvenMgr.h"
#include "../Manager/GISubsystem_StatMgr.h"
#include "../System/DataAsset/DA_ItemCategoryIcon.h"

void UGISubsystem_InvenMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UDataTable* ItemTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if ( IsValid(ItemTable) )
	{
		SetItemDataTable(ItemTable);
	}

	// 인벤토리 배열 크기 초기화
	FInvenItemMap InvenStorage;
	m_InvenStorage.Init({ InvenStorage }, (int32)EITEM_TYPE::END);
}

void UGISubsystem_InvenMgr::Deinitialize()
{
	OnClearInventoryList.Clear();
	OnAddInvenItem.Clear();
	OnRenewEquipItem.Clear();
	OnClearEquipList.Clear();
	OnAddEquipItemList.Clear();

	OnRenewQS.Clear();

	Super::Deinitialize();
}

void UGISubsystem_InvenMgr::SetItemDataTable(UDataTable* _ItemDataTable)
{
	//데이터 테이블의 정보를 TArray에 넣는다
	FString str;
	TArray<FGameItemInfo*> arrTableData;
	_ItemDataTable->GetAllRows<FGameItemInfo>(str, arrTableData);

	// 아이템 정보를 아이템 ID를 키값으로 하는 TMap에 넣는다
	for ( int32 i = 0; i < arrTableData.Num(); ++i )
	{
		m_MapItemInfo.Add(arrTableData[i]->ID, *arrTableData[i]);
	}
}

FGameItemInfo* UGISubsystem_InvenMgr::GetItemInfo(EITEM_ID _ID)
{
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);
	if ( pItemInfo == nullptr )
	{
		UE_LOG(LogTemp, Error, TEXT("GetItemInfo : 해당하는 아이템 정보를 찾을 수 없음"));
		return nullptr;
	}

	return pItemInfo;
}

FInvenItemRow* UGISubsystem_InvenMgr::GetInvenItemRow(EITEM_ID _ID)
{
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);
	if ( nullptr == pItemInfo )
	{
		UE_LOG(LogTemp, Error, TEXT("GetInvenItemRow : 해당하는 아이템 정보를 찾을 수 없음"));
		return nullptr;
	}

	FInvenItemRow* pInvenItemRow = m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Find(_ID);
	if ( nullptr == pInvenItemRow )
	{
		UE_LOG(LogTemp, Error, TEXT("GetInvenItemRow : 인벤토리에서 아이템 정보를 찾을 수 없음"));
		return nullptr;
	}

	return pInvenItemRow;
}

TOptional<TPair<uint32, FGameItemInfo*>> UGISubsystem_InvenMgr::GetEquipItemFromSlot(EEQUIP_SLOT _Slot)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(_Slot);
	if ( pItemRow == nullptr ) return TOptional<TPair<uint32, FGameItemInfo*>>();
	FGameItemInfo* pInfo = GetItemInfo(pItemRow->ID);
	if ( pInfo == nullptr ) return TOptional<TPair<uint32, FGameItemInfo*>>();

	return TPair<uint32, FGameItemInfo*>(pItemRow->Stack, pInfo);
}

FInvenItemRow* UGISubsystem_InvenMgr::AddGameItem(EITEM_ID _ID, uint32 _Stack)
{
	//획득한 아이템과 동일한 ID의 아이템 정보를 가져온다
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);
	if ( nullptr == pItemInfo )
	{
		UE_LOG(LogTemp, Error, TEXT("AddGameItem : 해당하는 아이템 정보를 찾을 수 없음"));
		return nullptr;
	}

	// 인벤토리에 해당 아이디의 아이템이 이미 존재하는지 검사
	// 없으면 인벤토리에 새 아이템을 추가한다.
	FInvenItemRow* ItemRow = m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Find(_ID);
	if ( ItemRow == nullptr )
	{
		FInvenItemRow* NewItem = &m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Add(_ID, FInvenItemRow{ _ID, _Stack, EEQUIP_SLOT::EMPTY });
		return NewItem;
	}

	// 있으면 인벤토리에 존재하는 아이템의 스택을 1 올린다. 
	ItemRow->Stack += _Stack;
	return ItemRow;
}

void UGISubsystem_InvenMgr::SubGameItem(EEQUIP_SLOT _Slot, EITEM_ID _ID)
{
	//삭제할 아이템과 동일한 ID의 아이템 정보를 가져온다
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);
	if ( nullptr == pItemInfo )
	{
		UE_LOG(LogTemp, Error, TEXT("SubGameItem : 해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	// 인벤토리에 해당 아이디의 아이템이 이미 존재하는지 검사
	// 없으면 아무것도 수행하지 않고, 있으면 인벤토리에서 삭제한다.
	FInvenItemRow* pItemRow = m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Find(_ID);
	if ( nullptr == pItemRow )
	{
		UE_LOG(LogTemp, Warning, TEXT("삭제할 아이템이 존재하지 않음"));
		return;
	}
	else
	{
		m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Remove(_ID);
	}
}

void UGISubsystem_InvenMgr::RenewInventoryUI(EITEM_TYPE _Type)
{
	// 인벤토리 위젯 내용 초기화
	OnClearInventoryList.Broadcast();
	// 인벤토리 매니저에서 보유중인 아이템목록을 인벤토리 위젯에 입력
	// 전체아이템 인벤토리일 경우
	if ( _Type == EITEM_TYPE::ALL )
	{
		for ( int32 i = 1; i < (int32)EITEM_TYPE::END; ++i )
		{
			for ( auto Iter = m_InvenStorage[i].StorageMap.CreateConstIterator(); Iter; ++Iter )
			{
				UItem_InvenData* pItemData = GetInvenDataToItemRow(Iter.Value());
				OnAddInvenItem.Broadcast(pItemData);
			}
		}
	}
	// 카테고리별 인벤토리일 경우
	else
	{
		for ( auto Iter = m_InvenStorage[ (int32)_Type ].StorageMap.CreateConstIterator(); Iter; ++Iter )
		{
			UItem_InvenData* pItemData = GetInvenDataToItemRow(Iter.Value());
			OnAddInvenItem.Broadcast(pItemData);
		}
	}
}

void UGISubsystem_InvenMgr::RenewEquipItemListUI(EITEM_TYPE _Type)
{
	OnClearEquipList.Broadcast();
	for ( auto Iter = m_InvenStorage[ (int32)_Type ].StorageMap.CreateConstIterator(); Iter; ++Iter )
	{
		UItem_InvenData* pItemData = GetInvenDataToItemRow(Iter.Value());
		OnAddEquipItemList.Broadcast(pItemData);
	}
}

void UGISubsystem_InvenMgr::ChangeEquipItem(EITEM_ID _ID, EEQUIP_SLOT _Slot)
{
	EITEM_TYPE _Type = GetItemTypeFromSlot(_Slot);
	FInvenItemRow* pItemRow = m_InvenStorage[ (int32)_Type ].StorageMap.Find(_ID);
	// 이미 해당슬롯에 장비중인 아이템을 다시 클릭할 경우 장비슬롯을 EMPTY로 바꾼다.
	if ( pItemRow->EquipedSlot == _Slot )
	{
		// 장비슬롯에서 장착해제 처리되어 아이템이 표시안되도록 변경한다.
		if ( _Type == EITEM_TYPE::CONSUMABLE )
		{
			UnEquipCurQuickSlot(_Slot);
		}
		else
		{
			UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
			if ( IsValid(StatMgr) )
			{
				FGameItemInfo* pInfo = m_MapItemInfo.Find(_ID);
				StatMgr->SetEquipFigure(pInfo, false);
				StatMgr->SetAtkAndDef();
			}
		}

		SetEquipSlotMap(nullptr, _Slot);
		pItemRow->EquipedSlot = EEQUIP_SLOT::EMPTY;
		RenewEquipItemListUI(_Type);
		OnRenewEquipItem.Broadcast(_Slot, FString());
		return;
	}

	// 장비슬롯에 아이템이 장착되는 경우
	FGameItemInfo* ItemInfo = m_MapItemInfo.Find(pItemRow->ID);
	// 해당 장비슬롯에 다른 아이템이 장착되어있을 경우 기존에 장착되어있던 아이템의 장비슬롯을 EMPTY로 바꾼다.
	FInvenItemRow* pSlotItemRow = m_EquipItemMap.Find(_Slot);
	if ( pSlotItemRow != nullptr )
	{
		FInvenItemRow* pEquipedItemRow = m_InvenStorage[ (int32)_Type ].StorageMap.Find(pSlotItemRow->ID);
		pEquipedItemRow->EquipedSlot = EEQUIP_SLOT::EMPTY;
		if ( ItemInfo->Type == EITEM_TYPE::CONSUMABLE )
		{
			UnEquipCurQuickSlot(_Slot);
		}
	}

	// 장착할 아이템이 다른 장비슬롯에 장비되어있을 경우 해당 장비슬롯에서 장착해제 처리한다.
	if ( pItemRow->EquipedSlot != EEQUIP_SLOT::EMPTY )
	{
		if ( ItemInfo->Type == EITEM_TYPE::CONSUMABLE )
		{
			UnEquipCurQuickSlot(pItemRow->EquipedSlot);
		}
		SetEquipSlotMap(nullptr, pItemRow->EquipedSlot);
		OnRenewEquipItem.Broadcast(pItemRow->EquipedSlot, FString());
	}

	// 아이템 장착 처리
	pItemRow->EquipedSlot = _Slot;
	RenewEquipItemListUI(_Type);
	SetEquipSlotMap(pItemRow, _Slot);
	UItem_InvenData* pItemData = GetInvenDataToItemRow(*pItemRow);
	FString ItemImgPath = GetItemImgFromSlot(pItemRow->EquipedSlot);
	OnRenewEquipItem.Broadcast(_Slot, ItemImgPath);	// 장비창 갱신

	if ( _Type == EITEM_TYPE::CONSUMABLE )
	{
		// 현재 퀵슬롯에 장착된 아이템이 없을 경우
		if ( GetQSItemForIndex(CurQuickSlotIdx) == nullptr )
		{
			// 지금 장착한 아이템을 현재 퀵슬롯에 설정
			int32 Index = ConvertQuickSlotToIdx(_Slot);
			CurQuickSlotIdx = Index;
		}
	}
	else
	{
		UGISubsystem_StatMgr* pStatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
		if ( IsValid(pStatMgr) )
		{
			FGameItemInfo* pInfo = m_MapItemInfo.Find(_ID);
			pStatMgr->SetEquipFigure(pInfo, true);
			pStatMgr->SetAtkAndDef();
		}
	}
}

void UGISubsystem_InvenMgr::UnEquipCurQuickSlot(EEQUIP_SLOT _Slot)
{
	if ( ConvertQuickSlotToIdx(_Slot) == CurQuickSlotIdx )
	{
		// 현재 퀵슬롯의 아이템을 장비해제 했을경우 다음 장착아이템이 있는 퀵슬롯으로 자리를 옮긴다.
		CurQuickSlotIdx = GetNextValidIndex();
	}
}

void UGISubsystem_InvenMgr::SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot)
{
	if ( _InvenItem == nullptr )
	{
		m_EquipItemMap.Remove(_Slot);
	}
	else
	{
		m_EquipItemMap.Emplace(_Slot, *_InvenItem);
	}
}

void UGISubsystem_InvenMgr::DecreaseInventoryItem(EITEM_ID _ID)
{
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);
	if ( nullptr == pItemInfo )
	{
		UE_LOG(LogTemp, Error, TEXT("DecreaseInventoryItem : 해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	// 인벤토리에 해당 아이디의 아이템이 이미 존재하는지 검사
	// 없으면 아무것도 수행하지 않고, 있으면 인벤토리에서 삭제한다. 
	FInvenItemRow* pItemRow = m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Find(_ID);
	if ( nullptr == pItemRow )
	{
		UE_LOG(LogTemp, Warning, TEXT("삭제할 아이템이 존재하지 않음"));
		return;
	}

	if ( pItemRow->Stack > 0 )
	{
		--pItemRow->Stack;
		if ( pItemRow->Stack <= 0 )
		{
			SubGameItem(pItemRow->EquipedSlot, pItemRow->ID);
		}
	}
}

void UGISubsystem_InvenMgr::DecreaseLowerSlotItem(EEQUIP_SLOT _Slot)
{
	FInvenItemRow* pItem = m_EquipItemMap.Find(_Slot);
	if ( nullptr == pItem )
	{
		UE_LOG(LogTemp, Error, TEXT("DecreaseLowerSlotItem : 해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	--pItem->Stack;
	if ( pItem->Stack == 0 )
	{
		// 장비창에서 아이템 표시 삭제
		UnEquipCurQuickSlot(_Slot);
		SetEquipSlotMap(nullptr, _Slot);
		OnRenewEquipItem.Broadcast(_Slot, FString());
	}
}

void UGISubsystem_InvenMgr::IncreaseEquipItemStack(EEQUIP_SLOT _Slot, int32 _Stack)
{
	FInvenItemRow* EquipItemRow = m_EquipItemMap.Find(_Slot);
	EquipItemRow->Stack += _Stack;
}

void UGISubsystem_InvenMgr::RenewQuickSlotUI()
{
	FInvenItemRow* CurSlotItem = GetQSItemForIndex(CurQuickSlotIdx);
	UItem_InvenData* pInvenItem = nullptr;
	UItem_InvenData* pNextInvenItem = nullptr;
	if ( CurSlotItem != nullptr )
	{
		pInvenItem = GetInvenDataToItemRow(*CurSlotItem);
	}	

	int32 NextIdx = GetNextValidIndex();
	if ( NextIdx != CurQuickSlotIdx )
	{
		FInvenItemRow* NextSlotItem = GetQSItemForIndex(NextIdx);
		pNextInvenItem = GetInvenDataToItemRow(*NextSlotItem);
	}

	OnRenewQS.Broadcast(pInvenItem, pNextInvenItem);
}

UItem_InvenData* UGISubsystem_InvenMgr::GetInvenDataToItemRow(const FInvenItemRow& _ItemRow)
{
	UItem_InvenData* ItemData = NewObject<UItem_InvenData>();
	FGameItemInfo* Info = m_MapItemInfo.Find(_ItemRow.ID);

	ItemData->SetItemImgPath(Info->IconImgPath);
	ItemData->SetItemName(Info->ItemName);
	ItemData->SetItemDesc(Info->Description);
	ItemData->SetItemQnt(_ItemRow.Stack);
	ItemData->SetPhysicAtkVal(Info->PhysicAtk);
	ItemData->SetPhysicDefVal(Info->PhysicDef);
	ItemData->SetMagicAtkVal(Info->MagicAtk);
	ItemData->SetMagicDefVal(Info->MagicDef);
	ItemData->SetRestoreHP(Info->Restore_HP);
	ItemData->SetRestoreMP(Info->Restore_MP);
	ItemData->SetRequireStr(Info->Require_Str);
	ItemData->SetRequireDex(Info->Require_Dex);
	ItemData->SetRequireInt(Info->Require_Int);
	ItemData->SetMaximumStack(Info->Maximum_Stack);
	ItemData->SetItemType(Info->Type);
	ItemData->SetEquiped(_ItemRow.EquipedSlot);
	ItemData->SetItemID(_ItemRow.ID);

	return ItemData;
}

FInvenItemRow* UGISubsystem_InvenMgr::GetQSItemForIndex(int32 _Idx)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(ConvertIdxToQuickSlot(_Idx));
	if ( pItemRow != nullptr )
	{
		return pItemRow;
	}

	return nullptr;
}

FInvenItemRow* UGISubsystem_InvenMgr::GetCurrentQSItem()
{
	return GetQSItemForIndex(CurQuickSlotIdx);
}

EITEM_TYPE UGISubsystem_InvenMgr::GetItemTypeFromSlot(EEQUIP_SLOT _Slot)
{
	EITEM_TYPE _Type = EITEM_TYPE::ALL;
	switch ( _Slot )
	{
	case EEQUIP_SLOT::WEAPON_1:
	case EEQUIP_SLOT::WEAPON_2:
	case EEQUIP_SLOT::WEAPON_3:
		_Type = EITEM_TYPE::WEAPON;
		break;
	case EEQUIP_SLOT::SHIELD_1:
	case EEQUIP_SLOT::SHIELD_2:
	case EEQUIP_SLOT::SHIELD_3:
		_Type = EITEM_TYPE::SHIELD;
		break;
	case EEQUIP_SLOT::ARROW:
		_Type = EITEM_TYPE::ARROWS;
		break;
	case EEQUIP_SLOT::BOLT:
		_Type = EITEM_TYPE::ARROWS;
		break;
	case EEQUIP_SLOT::HELM:
		_Type = EITEM_TYPE::ARM_HELM;
		break;
	case EEQUIP_SLOT::CHEST:
		_Type = EITEM_TYPE::ARM_CHEST;
		break;
	case EEQUIP_SLOT::GAUNTLET:
		_Type = EITEM_TYPE::ARM_GAUNTLET;
		break;
	case EEQUIP_SLOT::LEGGINGS:
		_Type = EITEM_TYPE::ARM_LEGGINGS;
		break;
	case EEQUIP_SLOT::ACCESSORIE_1:
	case EEQUIP_SLOT::ACCESSORIE_2:
	case EEQUIP_SLOT::ACCESSORIE_3:
	case EEQUIP_SLOT::ACCESSORIE_4:
		_Type = EITEM_TYPE::ACCESSORIE;
		break;
	case EEQUIP_SLOT::CONSUMABLE_1:
	case EEQUIP_SLOT::CONSUMABLE_2:
	case EEQUIP_SLOT::CONSUMABLE_3:
	case EEQUIP_SLOT::CONSUMABLE_4:
	case EEQUIP_SLOT::CONSUMABLE_5:
		_Type = EITEM_TYPE::CONSUMABLE;
		break;
	default:
		break;
	}

	return _Type;
}

int32 UGISubsystem_InvenMgr::GetNextValidIndex()
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
			UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 등록된 아이템 하나거나 없음"));
			break;
		}
	}
	return idx;
}

int32 UGISubsystem_InvenMgr::ConvertQuickSlotToIdx(EEQUIP_SLOT _Slot)
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

EEQUIP_SLOT UGISubsystem_InvenMgr::ConvertIdxToQuickSlot(int32 _Idx)
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

FString UGISubsystem_InvenMgr::GetItemImgFromSlot(EEQUIP_SLOT _Slot)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(_Slot);
	if ( pItemRow == nullptr ) return FString();
	FGameItemInfo* pInfo = GetItemInfo(pItemRow->ID);
	if ( pInfo == nullptr ) return FString();

	return pInfo->IconImgPath;
}
