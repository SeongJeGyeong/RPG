// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_InvenMgr.h"
#include "../Manager/GISubsystem_StatMgr.h"
#include "../System/DataAsset/DA_ItemCategoryIcon.h"
#include "../Manager/GISubsystem_EquipMgr.h"

void UGISubsystem_InvenMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UDataTable* ItemTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if ( IsValid(ItemTable) )
	{
		SetItemDataTable(ItemTable);
	}
	UDataAsset* ItemIcon = LoadObject<UDataAsset>(nullptr, TEXT("/Script/RPGPortfolio.DA_ItemCategoryIcon'/Game/Blueprint/DataAsset/BPC_DA_CategoryIcon.BPC_DA_CategoryIcon'"));
	if ( IsValid(ItemIcon) )
	{
		SetInventoryIcon(ItemIcon);
	}
}

void UGISubsystem_InvenMgr::Deinitialize()
{
	OnInventoryOpen.Clear();
	OnClearInventoryList.Clear();
	OnAddInvenItem.Clear();
	OnRenewEquipItem.Clear();
	OnClearEquipList.Clear();
	OnAddEquipItemList.Clear();

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
	// 인벤토리 배열 크기 초기화
	FInvenItemMap InvenStorage;
	m_InvenStorage.Init({ InvenStorage }, (int32)EITEM_TYPE::END);
}

void UGISubsystem_InvenMgr::SetInventoryIcon(UDataAsset* _DataAsset)
{
	m_Icon = Cast<UDA_ItemCategoryIcon>(_DataAsset);
	if ( IsValid(m_Icon) )
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리 아이콘 로드 성공"));
	}
}

FGameItemInfo* UGISubsystem_InvenMgr::GetItemInfo(EITEM_ID _ID)
{
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);
	if ( nullptr == pItemInfo )
	{
		UE_LOG(LogTemp, Error, TEXT("GetItemInfo : 해당하는 아이템 정보를 찾을 수 없음"));
		return nullptr;
	}

	return pItemInfo;
}

FInvenItemRow* UGISubsystem_InvenMgr::GetInvenItemInfo(EITEM_ID _ID)
{
	FGameItemInfo* pItemInfo = GetItemInfo(_ID);
	FInvenItemRow* pInvenItemRow = m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Find(_ID);
	if ( nullptr == pInvenItemRow )
	{
		UE_LOG(LogTemp, Error, TEXT("GetInvenItemInfo : 해당하는 아이템 정보를 찾을 수 없음"));
		return nullptr;
	}

	return pInvenItemRow;
}

UItem_InvenData* UGISubsystem_InvenMgr::GetEquipItemFromSlot(EEQUIP_SLOT _Slot)
{
	FInvenItemRow* pItemRow = m_EquipItemMap.Find(_Slot);
	if ( pItemRow == nullptr )
	{
		return nullptr;
	}

	UItem_InvenData* pItemData = GetInvenDataToItemRow(*pItemRow);
	return pItemData;
}

void UGISubsystem_InvenMgr::AddGameItem(EITEM_ID _ID, uint32 _Stack)
{
	//획득한 아이템과 동일한 ID의 아이템 정보를 가져온다
	FGameItemInfo* pItemInfo = GetItemInfo(_ID);

	// 인벤토리에 해당 아이디의 아이템이 이미 존재하는지 검사
	// 없으면 인벤토리에 새 아이템을 추가한다.
	// 있으면 인벤토리에 존재 하는 아이템의 스택을 1 올린다. 
	FInvenItemRow* ItemRow = m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Find(_ID);
	if ( ItemRow == nullptr )
	{
		m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Add(_ID, FInvenItemRow{ _ID, _Stack, EEQUIP_SLOT::EMPTY });
	}
	else
	{
		ItemRow->Stack += _Stack;
		// 퀵슬롯에 등록된 아이템이 추가되었을 경우 퀵슬롯의 아이템 개수를 갱신
		if ( pItemInfo->Type == EITEM_TYPE::CONSUMABLE && ItemRow->EquipedSlot != EEQUIP_SLOT::EMPTY )
		{
			UGISubsystem_EquipMgr* pEquipMgr = GetGameInstance()->GetSubsystem<UGISubsystem_EquipMgr>();
			if ( IsValid(pEquipMgr) )
			{
				int32 idx = pEquipMgr->ConvertQuickSlotToIdx(ItemRow->EquipedSlot);
				pEquipMgr->AddStackQuickSlot(ItemRow->EquipedSlot, _Stack);
				pEquipMgr->RenewQuickSlotUI(idx);
			}
		}
	}
}

void UGISubsystem_InvenMgr::SubGameItem(EEQUIP_SLOT _Slot, EITEM_ID _ID)
{
	//삭제할 아이템과 동일한 ID의 아이템 정보를 가져온다
	FGameItemInfo* pItemInfo = GetItemInfo(_ID);

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
		if ( _Slot != EEQUIP_SLOT::EMPTY )
		{
			if ( pItemInfo->Type == EITEM_TYPE::CONSUMABLE )
			{
				// 장비창에서 아이템 표시 삭제
				UnEquipConsumeUI(_Slot);
			}
		}
		m_InvenStorage[ (int32)pItemInfo->Type ].StorageMap.Remove(_ID);
	}
}

void UGISubsystem_InvenMgr::ShowInventoryUI()
{
	OnInventoryOpen.Broadcast(true);
	RenewInventoryUI(EITEM_TYPE::ALL);
}

void UGISubsystem_InvenMgr::CloseInventoryUI()
{
	OnInventoryOpen.Broadcast(false);
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

	FInvenItemRow* pItemRow = m_InvenStorage[ (int32)_Type ].StorageMap.Find(_ID);

	// 이미 해당슬롯에 장비중인 아이템을 다시 클릭할 경우 장비슬롯을 EMPTY로 바꾼다.
	if ( pItemRow->EquipedSlot == _Slot )
	{
		pItemRow->EquipedSlot = EEQUIP_SLOT::EMPTY;
		RenewEquipItemListUI(_Type);
		// 장비슬롯에서 장착해제 처리되어 아이템이 표시안되도록 변경한다.
		SetEquipSlotMap(nullptr, _Slot);
		if ( _Type == EITEM_TYPE::CONSUMABLE )
		{
			UnEquipConsumeUI(_Slot);
		}
		else
		{
			RenewEquipItemUI(_Slot, nullptr);

			UGISubsystem_StatMgr* StatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
			if ( IsValid(StatMgr) )
			{
				FGameItemInfo* pInfo = GetItemInfo(_ID);
				StatMgr->SetEquipFigure(pInfo, false);
				StatMgr->SetAtkAndDef();
			}
		}

		return;
	}
	// 장비슬롯에 아이템이 장착되는 경우
	else
	{
		FGameItemInfo* pInfo = GetItemInfo(pItemRow->ID);
		// 해당 장비슬롯에 다른 아이템이 장착되어있을 경우 기존에 장착되어있던 아이템의 장비슬롯을 EMPTY로 바꾼다.
		FInvenItemRow* pSlotItemRow = m_EquipItemMap.Find(_Slot);
		if ( pSlotItemRow != nullptr )
		{
			FInvenItemRow* pEquipedItemRow = m_InvenStorage[ (int32)_Type ].StorageMap.Find(pSlotItemRow->ID);
			if ( pInfo->Type == EITEM_TYPE::CONSUMABLE )
			{
				// 기존에 장비되어있던 아이템 퀵슬롯에서 해제
				UnEquipConsumeUI(pEquipedItemRow->EquipedSlot);
			}
			else
			{
				RenewEquipItemUI(pEquipedItemRow->EquipedSlot, nullptr);
			}

			pEquipedItemRow->EquipedSlot = EEQUIP_SLOT::EMPTY;
		}

		// 장착할 아이템이 다른 장비슬롯에 장비되어있을 경우 해당 장비슬롯에서 장착해제 처리한다.
		if ( pItemRow->EquipedSlot != EEQUIP_SLOT::EMPTY )
		{
			if ( pInfo->Type == EITEM_TYPE::CONSUMABLE )
			{
				UnEquipConsumeUI(pItemRow->EquipedSlot);
			}
			else
			{
				RenewEquipItemUI(pItemRow->EquipedSlot, nullptr);
			}
		}
		pItemRow->EquipedSlot = _Slot;
	}

	RenewEquipItemListUI(_Type);

	SetEquipSlotMap(pItemRow, _Slot);
	if ( _Type == EITEM_TYPE::CONSUMABLE )
	{
		EquipConsumeUI(_Slot, *pItemRow);
	}
	else
	{
		RenewEquipItemUI(_Slot, pItemRow);

		UGISubsystem_StatMgr* pStatMgr = GetGameInstance()->GetSubsystem<UGISubsystem_StatMgr>();
		if ( IsValid(pStatMgr) )
		{
			FGameItemInfo* pInfo = GetItemInfo(_ID);
			pStatMgr->SetEquipFigure(pInfo, true);
			pStatMgr->SetAtkAndDef();
		}
	}
}

void UGISubsystem_InvenMgr::EquipConsumeUI(EEQUIP_SLOT _Slot, const FInvenItemRow& _ItemRow)
{
	UItem_InvenData* pItemData = GetInvenDataToItemRow(_ItemRow);
	OnRenewEquipItem.Broadcast(_Slot, pItemData);

	UGISubsystem_EquipMgr* pEquipMgr = GetGameInstance()->GetSubsystem<UGISubsystem_EquipMgr>();
	if ( IsValid(pEquipMgr) ) pEquipMgr->EquipQuickSlotArray(_ItemRow, _Slot);
}

void UGISubsystem_InvenMgr::UnEquipConsumeUI(EEQUIP_SLOT _Slot)
{
	OnRenewEquipItem.Broadcast(_Slot, nullptr);

	UGISubsystem_EquipMgr* pEquipMgr = GetGameInstance()->GetSubsystem<UGISubsystem_EquipMgr>();
	if ( IsValid(pEquipMgr) ) pEquipMgr->UnEquipQuickSlotArray(_Slot);
}

void UGISubsystem_InvenMgr::RenewEquipItemUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow)
{
	if ( _ItemRow == nullptr )
	{
		OnRenewEquipItem.Broadcast(_Slot, nullptr);

	}
	else
	{
		UItem_InvenData* pItemData = GetInvenDataToItemRow(*_ItemRow);
		OnRenewEquipItem.Broadcast(_Slot, pItemData);
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

UPaperSprite* UGISubsystem_InvenMgr::GetCategoryIcon(EITEM_TYPE _type)
{
	return 	m_Icon->GetCategoryIcon(_type);
}

UPaperSprite* UGISubsystem_InvenMgr::GetEquipSlotIcon(EEQUIP_SLOT _Slot)
{
	return m_Icon->GetEquipSlotIcon(_Slot);
}

UItem_InvenData* UGISubsystem_InvenMgr::GetInvenDataToItemRow(const FInvenItemRow& _ItemRow)
{
	UItem_InvenData* ItemData = NewObject<UItem_InvenData>();
	FGameItemInfo* Info = GetItemInfo(_ItemRow.ID);

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
