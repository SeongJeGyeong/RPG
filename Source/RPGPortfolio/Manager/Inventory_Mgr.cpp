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
				pItemData->SetAtkVal(Iter.Value().ItemInfo->ATK);
				pItemData->SetDefVal(Iter.Value().ItemInfo->DEF);
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
			pItemData->SetAtkVal(Iter.Value().ItemInfo->ATK);
			pItemData->SetDefVal(Iter.Value().ItemInfo->DEF);
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
		pItemData->SetAtkVal(Iter.Value().ItemInfo->ATK);
		pItemData->SetDefVal(Iter.Value().ItemInfo->DEF);
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

bool UInventory_Mgr::GetInvenStorage(TMap<EITEM_ID, FInvenItemRow>& _OutInvenStorage, int32 _Idx)
{
	_OutInvenStorage = m_InvenStorage[_Idx];
	return true;
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

	if (m_InvenStorage[(int32)_Type].Find(_ID)->EquipedSlot == _Slot)
	{
		FInvenItemRow* pItemRow = m_InvenStorage[(int32)_Type].Find(_ID);
		pItemRow->EquipedSlot = EEQUIP_SLOT::EMPTY;
		m_InvenStorage[(int32)_Type].Add(_ID, *pItemRow);
		RenewItemListUI(_Type);
		return;
	}

	for (auto Iter = m_InvenStorage[(int32)_Type].CreateIterator(); Iter; ++Iter)
	{
		if (Iter.Key() != _ID && Iter.Value().EquipedSlot == _Slot)
		{
			Iter.Value().EquipedSlot = EEQUIP_SLOT::EMPTY;
		}
		else if (Iter.Key() == _ID)
		{
			Iter.Value().EquipedSlot = _Slot;
		}
	}

	RenewItemListUI(_Type);
}