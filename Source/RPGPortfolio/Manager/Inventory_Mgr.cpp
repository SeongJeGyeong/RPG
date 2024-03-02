// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Mgr.h"
#include "../GameInstance_Base.h"
#include "../UI/UI_Inventory.h"
#include "../UI/UI_EquipMain.h"
#include "../UI/UI_EquipItemList.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/UI_Base.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Equip_Mgr.h"
#include "../System/PlayerState_Base.h"

UWorld* UInventory_Mgr::m_World = nullptr;

UInventory_Mgr* UInventory_Mgr::GetInst(UWorld* _World)
{
	m_World = _World;

	return GetInst(m_World->GetGameInstance());
}

UInventory_Mgr* UInventory_Mgr::GetInst(UGameInstance* _GameInst)
{
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(_GameInst);

	if (!IsValid(pGameInst->m_InvenMgr))
	{
		pGameInst->m_InvenMgr = NewObject<UInventory_Mgr>();
		
		// AddToRoot : 객체를 루트에 추가함
		// 루트에 존재하는 객체는 가비지컬렉터가 자동으로 삭제하지 않기 때문에
		// 대신 사용하지 않을 때는 수동으로 삭제해줘야함
		pGameInst->m_InvenMgr->AddToRoot();
	}

	return pGameInst->m_InvenMgr;
}

void UInventory_Mgr::SetItemDataTable(UDataTable* _ItemDataTable)
{
	m_ItemDataTable = _ItemDataTable;

	//데이터 테이블의 정보를 TArray에 넣는다
	FString str;
	TArray<FGameItemInfo*> arrTableData;
	m_ItemDataTable->GetAllRows<FGameItemInfo>(str, arrTableData);

	// 아이템 정보를 아이템 ID를 키값으로 하는 TMap에 넣는다
	for ( int32 i = 0; i < arrTableData.Num(); ++i )
	{
		m_MapItemInfo.Add(arrTableData[i]->ID, *arrTableData[i]);
	}
}

void UInventory_Mgr::AddGameItem(EITEM_ID _ID)
{
	//습득한 아이템과 동일한 ID의 아이템 정보를 가져온다
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);

	if ( nullptr == pItemInfo )
	{
		UE_LOG(LogTemp, Error, TEXT("해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	// 인벤토리에 해당 아이디의 아이템이 이미 존재하는지 검사
	// 없으면 인벤토리에 새 아이템을 추가한다.
	// 있으면 인벤토리에 존재 하는 아이템의 스택을 1 올린다. 
	FInvenItemRow* pItemRow = m_InvenStorage[(int32)pItemInfo->Type].Find(_ID);
	if ( nullptr == pItemRow )
	{
		m_InvenStorage[(int32)pItemInfo->Type].Add(_ID, FInvenItemRow{pItemInfo, 1, EEQUIP_SLOT::EMPTY});
	}
	else
	{
		++pItemRow->Stack;
	}

	//인벤토리  UI가 열려있을 경우 갱신
	/*if (CheckInventoryOpened())
	{
		RenewInventoryUI(EITEM_TYPE::ALL);
	}*/
}

void UInventory_Mgr::SubGameItem(EEQUIP_SLOT _Slot, EITEM_ID _ID)
{
	//삭제할 아이템과 동일한 ID의 아이템 정보를 가져온다
	FGameItemInfo* pItemInfo = m_MapItemInfo.Find(_ID);

	if (nullptr == pItemInfo)
	{
		UE_LOG(LogTemp, Error, TEXT("해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	// 인벤토리에 해당 아이디의 아이템이 이미 존재하는지 검사
	// 없으면 아무것도 수행하지 않고, 있으면 인벤토리에서 삭제한다. 
	FInvenItemRow* pItemRow = m_InvenStorage[(int32)pItemInfo->Type].Find(_ID);
	if (nullptr == pItemRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("삭제할 아이템이 존재하지 않음"));
		return;
	}
	else
	{
		RenewEquipConsumeUI(_Slot, pItemRow, true);
		m_InvenStorage[(int32)pItemInfo->Type].Remove(_ID);
	}
}

void UInventory_Mgr::ShowInventoryUI()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));

	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}

	UUI_Inventory* InventoryUI = GameMode->GetInventoryUI();
	InventoryUI->SetStatUI(UGameplayStatics::GetPlayerState(m_World, 0));
	RenewInventoryUI(EITEM_TYPE::ALL);
	InventoryUI->SetVisibility(ESlateVisibility::Visible);
}

void UInventory_Mgr::CloseInventoryUI()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));

	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}

	UUI_Inventory* InventoryUI = GameMode->GetInventoryUI();
	InventoryUI->SetVisibility(ESlateVisibility::Hidden);
}

void UInventory_Mgr::RenewInventoryUI(EITEM_TYPE _Type)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));

	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}
	UUI_Inventory* InventoryUI = GameMode->GetInventoryUI();

	// 인벤토리 위젯 내용 초기화
	InventoryUI->Clear();

	// 인벤토리 매니저에서 보유중인 아이템목록을 인벤토리 위젯에 입력
	// 전체아이템 인벤토리일 경우
	if (_Type == EITEM_TYPE::ALL)
	{
		for (int32 i = 1; i < (int32)EITEM_TYPE::END; ++i)
		{
			for (auto Iter = m_InvenStorage[i].CreateConstIterator(); Iter; ++Iter)
			{
				UItem_InvenData* pItemData = NewObject<UItem_InvenData>();

				pItemData->SetItemImgPath(Iter.Value().ItemInfo->IconImgPath);
				pItemData->SetItemName(Iter.Value().ItemInfo->ItemName);
				pItemData->SetItemDesc(Iter.Value().ItemInfo->Description);
				pItemData->SetItemQnt(Iter.Value().Stack);
				pItemData->SetPhysicAtkVal(Iter.Value().ItemInfo->PhysicAtk);
				pItemData->SetPhysicDefVal(Iter.Value().ItemInfo->PhysicDef);
				pItemData->SetMagicAtkVal(Iter.Value().ItemInfo->MagicAtk);
				pItemData->SetMagicDefVal(Iter.Value().ItemInfo->MagicDef);
				pItemData->SetRestoreHP(Iter.Value().ItemInfo->Restore_HP);
				pItemData->SetRestoreMP(Iter.Value().ItemInfo->Restore_MP);
				pItemData->SetRequireStr(Iter.Value().ItemInfo->Require_Str);
				pItemData->SetRequireDex(Iter.Value().ItemInfo->Require_Dex);
				pItemData->SetRequireInt(Iter.Value().ItemInfo->Require_Int);
				pItemData->SetMaximumStack(Iter.Value().ItemInfo->Maximum_Stack);
				pItemData->SetItemType(Iter.Value().ItemInfo->Type);
				pItemData->SetEquiped(Iter.Value().EquipedSlot);
				pItemData->SetItemID(Iter.Key());

				InventoryUI->AddInventoryItem(pItemData);
			}
		}
	}
	// 카테고리별 인벤토리일 경우
	else
	{
		for (auto Iter = m_InvenStorage[(int32)_Type].CreateConstIterator(); Iter; ++Iter)
		{
			UItem_InvenData* pItemData = NewObject<UItem_InvenData>();

			pItemData->SetItemImgPath(Iter.Value().ItemInfo->IconImgPath);
			pItemData->SetItemName(Iter.Value().ItemInfo->ItemName);
			pItemData->SetItemDesc(Iter.Value().ItemInfo->Description);
			pItemData->SetItemQnt(Iter.Value().Stack);
			pItemData->SetPhysicAtkVal(Iter.Value().ItemInfo->PhysicAtk);
			pItemData->SetPhysicDefVal(Iter.Value().ItemInfo->PhysicDef);
			pItemData->SetMagicAtkVal(Iter.Value().ItemInfo->MagicAtk);
			pItemData->SetMagicDefVal(Iter.Value().ItemInfo->MagicDef);
			pItemData->SetRestoreHP(Iter.Value().ItemInfo->Restore_HP);
			pItemData->SetRestoreMP(Iter.Value().ItemInfo->Restore_MP);
			pItemData->SetRequireStr(Iter.Value().ItemInfo->Require_Str);
			pItemData->SetRequireDex(Iter.Value().ItemInfo->Require_Dex);
			pItemData->SetRequireInt(Iter.Value().ItemInfo->Require_Int);
			pItemData->SetMaximumStack(Iter.Value().ItemInfo->Maximum_Stack);
			pItemData->SetItemType(Iter.Value().ItemInfo->Type);
			pItemData->SetEquiped(Iter.Value().EquipedSlot);
			pItemData->SetItemID(Iter.Key());
			
			InventoryUI->AddInventoryItem(pItemData);
		}
	}
}

void UInventory_Mgr::RenewItemListUI(EITEM_TYPE _Type)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));

	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}
	UUI_EquipMain* EquipMainUI = GameMode->GetEquipUI();

	UUI_EquipItemList* EquipItemListUI = EquipMainUI->GetItemList();
	EquipItemListUI->ClearTileView();

	for (auto Iter = m_InvenStorage[(int32)_Type].CreateConstIterator(); Iter; ++Iter)
	{
		UItem_InvenData* pItemData = NewObject<UItem_InvenData>();

		pItemData->SetItemImgPath(Iter.Value().ItemInfo->IconImgPath);
		pItemData->SetItemName(Iter.Value().ItemInfo->ItemName);
		pItemData->SetItemDesc(Iter.Value().ItemInfo->Description);
		pItemData->SetItemQnt(Iter.Value().Stack);
		pItemData->SetPhysicAtkVal(Iter.Value().ItemInfo->PhysicAtk);
		pItemData->SetPhysicDefVal(Iter.Value().ItemInfo->PhysicDef);
		pItemData->SetMagicAtkVal(Iter.Value().ItemInfo->MagicAtk);
		pItemData->SetMagicDefVal(Iter.Value().ItemInfo->MagicDef);
		pItemData->SetRestoreHP(Iter.Value().ItemInfo->Restore_HP);
		pItemData->SetRestoreMP(Iter.Value().ItemInfo->Restore_MP);
		pItemData->SetRequireStr(Iter.Value().ItemInfo->Require_Str);
		pItemData->SetRequireDex(Iter.Value().ItemInfo->Require_Dex);
		pItemData->SetRequireInt(Iter.Value().ItemInfo->Require_Int);
		pItemData->SetMaximumStack(Iter.Value().ItemInfo->Maximum_Stack);
		pItemData->SetItemType(Iter.Value().ItemInfo->Type);
		pItemData->SetEquiped(Iter.Value().EquipedSlot);
		pItemData->SetItemID(Iter.Key());

		EquipItemListUI->AddEquipItemList(pItemData);
	}
}

bool UInventory_Mgr::CheckInventoryOpened()
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return false;
	}
	UUI_Inventory* InventoryUI = GameMode->GetInventoryUI();

	return InventoryUI->IsInventoryOpened();
}

void UInventory_Mgr::ChangeEquipItem(EITEM_ID _ID, EEQUIP_SLOT _Slot)
{
	EITEM_TYPE _Type;
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

	// 이미 해당슬롯에 장비중인 아이템을 다시 클릭할 경우 장비슬롯을 EMPTY로 바꾼 후 스토리지에 다시 넣는다.
	if (m_InvenStorage[(int32)_Type].Find(_ID)->EquipedSlot == _Slot)
	{
		FInvenItemRow* pItemRow = m_InvenStorage[(int32)_Type].Find(_ID);
		pItemRow->EquipedSlot = EEQUIP_SLOT::EMPTY;
		m_InvenStorage[(int32)_Type].Add(_ID, *pItemRow);
		RenewItemListUI(_Type);
		// 장비슬롯에서 장착해제 처리되어 아이템이 표시안되도록 변경한다. 
		if (_Type == EITEM_TYPE::CONSUMABLE)
		{
			RenewEquipConsumeUI(_Slot, pItemRow, true);
		}
		else
		{
			RenewEquipItemUI(_Slot, nullptr);
			UEquip_Mgr::GetInst(m_World)->SetEquipSlotMap(nullptr, _Slot);
			
			APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(m_World, 0));
			
			pPlayerState->SetEquipFigure(pItemRow->ItemInfo, false);
			pPlayerState->SetPlayerBasePower();
		}

		return;
	}

	for (auto Iter = m_InvenStorage[(int32)_Type].CreateIterator(); Iter; ++Iter)
	{
		// 해당 장비슬롯에 다른 아이템이 장비되어있을 경우 기존에 장비되어있던 아이템의 장비슬롯을 EMPTY로 바꾼다.
		if (Iter.Key() != _ID && Iter.Value().EquipedSlot == _Slot)
		{
			if ( Iter.Value().ItemInfo->Type == EITEM_TYPE::CONSUMABLE )
			{
				// 기존에 장비되어있던 아이템 퀵슬롯에서 해제
				RenewEquipConsumeUI(Iter.Value().EquipedSlot, m_InvenStorage[ (int32)_Type ].Find(Iter.Key()), true);
			}
			else
			{
				RenewEquipItemUI(Iter.Value().EquipedSlot, nullptr);
			}

			Iter.Value().EquipedSlot = EEQUIP_SLOT::EMPTY;
		}

		if (Iter.Key() == _ID)
		{
			// 장착할 아이템이 다른 장비슬롯에 장비되어있을 경우 해당 장비슬롯에서 장착해제 처리한다.
			if (Iter.Value().EquipedSlot != EEQUIP_SLOT::EMPTY)
			{
				if ( Iter.Value().ItemInfo->Type == EITEM_TYPE::CONSUMABLE )
				{
					RenewEquipConsumeUI(Iter.Value().EquipedSlot, m_InvenStorage[(int32)_Type].Find(_ID), true);
				}
				else
				{
					RenewEquipItemUI(Iter.Value().EquipedSlot, nullptr);
				}
			}

			Iter.Value().EquipedSlot = _Slot;
		}
	}
	RenewItemListUI(_Type);

	FInvenItemRow* pItemRow = m_InvenStorage[ (int32)_Type ].Find(_ID);

	if (_Type == EITEM_TYPE::CONSUMABLE)
	{
		RenewEquipConsumeUI(_Slot, pItemRow, false);
	}
	else
	{
		RenewEquipItemUI(_Slot, pItemRow);
		UEquip_Mgr::GetInst(m_World)->SetEquipSlotMap(pItemRow, _Slot);

		APlayerState_Base* pPlayerState = Cast<APlayerState_Base>(UGameplayStatics::GetPlayerState(m_World, 0));
		pPlayerState->SetEquipFigure(pItemRow->ItemInfo, true);
		pPlayerState->SetPlayerBasePower();
	}

}

void UInventory_Mgr::RenewEquipConsumeUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow, bool _Unequip)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));

	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}
	UUI_EquipMain* EquipMainUI = GameMode->GetEquipUI();

	// 장비슬롯 열거형을 인덱스로 변환
	int32 Index = UEquip_Mgr::GetInst(m_World)->ConvertQuickSlotToIdx(_Slot);
	UE_LOG(LogTemp, Display, TEXT("지정된 슬롯 인덱스 : %d"), Index);

	//장비해제인 경우
	if (_Unequip)
	{
		EquipMainUI->RenewEquipItem(_Slot);
		UEquip_Mgr::GetInst(m_World)->SetQuickSlotArray(_ItemRow, Index, true);

		int32 curIdx = UEquip_Mgr::GetInst(m_World)->GetCurrentIndex();
		UEquip_Mgr::GetInst(m_World)->RenewQuickSlotUI(curIdx);

		return;
	}

	UItem_InvenData* pItemData = NewObject<UItem_InvenData>();
	pItemData->SetItemImgPath(_ItemRow->ItemInfo->IconImgPath);
	pItemData->SetItemName(_ItemRow->ItemInfo->ItemName);
	pItemData->SetItemDesc(_ItemRow->ItemInfo->Description);
	pItemData->SetItemQnt(_ItemRow->Stack);
	pItemData->SetPhysicAtkVal(_ItemRow->ItemInfo->PhysicAtk);
	pItemData->SetPhysicDefVal(_ItemRow->ItemInfo->PhysicDef);
	pItemData->SetMagicAtkVal(_ItemRow->ItemInfo->MagicAtk);
	pItemData->SetMagicDefVal(_ItemRow->ItemInfo->MagicDef);
	pItemData->SetRestoreHP(_ItemRow->ItemInfo->Restore_HP);
	pItemData->SetRestoreMP(_ItemRow->ItemInfo->Restore_MP);
	pItemData->SetRequireStr(_ItemRow->ItemInfo->Require_Str);
	pItemData->SetRequireDex(_ItemRow->ItemInfo->Require_Dex);
	pItemData->SetRequireInt(_ItemRow->ItemInfo->Require_Int);
	pItemData->SetMaximumStack(_ItemRow->ItemInfo->Maximum_Stack);
	pItemData->SetItemType(_ItemRow->ItemInfo->Type);
	pItemData->SetEquiped(_ItemRow->EquipedSlot);
	pItemData->SetItemID(_ItemRow->ItemInfo->ID);

	EquipMainUI->RenewEquipItem(_Slot, pItemData);

	UEquip_Mgr::GetInst(m_World)->SetQuickSlotArray(_ItemRow, Index, false);

	int32 curIdx = UEquip_Mgr::GetInst(m_World)->GetCurrentIndex();
	UE_LOG(LogTemp, Display, TEXT("현재 슬롯 인덱스 : %d"), curIdx);
	if (Index == curIdx)
	{
		UEquip_Mgr::GetInst(m_World)->RenewQuickSlotUI(Index);
	}
	else
	{
		int32 nextIdx = UEquip_Mgr::GetInst(m_World)->GetNextArrayIndex();
		if (curIdx != nextIdx)
		{
			UEquip_Mgr::GetInst(m_World)->RenewQuickSlotUI(curIdx);
		}
	}
}

void UInventory_Mgr::RenewEquipItemUI(EEQUIP_SLOT _Slot, FInvenItemRow* _ItemRow)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));

	if (!IsValid(GameMode))
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}
	UUI_EquipMain* EquipMainUI = GameMode->GetEquipUI();

	if (_ItemRow == nullptr)
	{
		EquipMainUI->RenewEquipItem(_Slot);
		return;
	}

	UItem_InvenData* pItemData = NewObject<UItem_InvenData>();
	pItemData->SetItemImgPath(_ItemRow->ItemInfo->IconImgPath);
	pItemData->SetItemName(_ItemRow->ItemInfo->ItemName);
	pItemData->SetItemDesc(_ItemRow->ItemInfo->Description);
	pItemData->SetItemQnt(_ItemRow->Stack);
	pItemData->SetPhysicAtkVal(_ItemRow->ItemInfo->PhysicAtk);
	pItemData->SetPhysicDefVal(_ItemRow->ItemInfo->PhysicDef);
	pItemData->SetMagicAtkVal(_ItemRow->ItemInfo->MagicAtk);
	pItemData->SetMagicDefVal(_ItemRow->ItemInfo->MagicDef);
	pItemData->SetRestoreHP(_ItemRow->ItemInfo->Restore_HP);
	pItemData->SetRestoreMP(_ItemRow->ItemInfo->Restore_MP);
	pItemData->SetRequireStr(_ItemRow->ItemInfo->Require_Str);
	pItemData->SetRequireDex(_ItemRow->ItemInfo->Require_Dex);
	pItemData->SetRequireInt(_ItemRow->ItemInfo->Require_Int);
	pItemData->SetMaximumStack(_ItemRow->ItemInfo->Maximum_Stack);
	pItemData->SetItemType(_ItemRow->ItemInfo->Type);
	pItemData->SetEquiped(_ItemRow->EquipedSlot);
	pItemData->SetItemID(_ItemRow->ItemInfo->ID);

	EquipMainUI->RenewEquipItem(_Slot, pItemData);
}