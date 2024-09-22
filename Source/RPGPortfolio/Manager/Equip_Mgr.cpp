// Fill out your copyright notice in the Description page of Project Settings.


#include "Equip_Mgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"
#include "../UI/UI_Player_QuickSlot.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_StatusMain.h"
#include "../UI/UI_EquipMain.h"
#include "Inventory_Mgr.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"

UWorld* UEquip_Mgr::m_World = nullptr;

UEquip_Mgr* UEquip_Mgr::GetInst(UWorld* _World)
{
    m_World = _World;
	
    return GetInst(m_World->GetGameInstance());
}

UEquip_Mgr* UEquip_Mgr::GetInst(UGameInstance* _GameInst)
{
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(_GameInst);

	if (!IsValid(pGameInst->m_EquipMgr))
	{
		pGameInst->m_EquipMgr = NewObject<UEquip_Mgr>();
		pGameInst->m_EquipMgr->AddToRoot();
	}

	return pGameInst->m_EquipMgr;
}

TSharedPtr<FInvenItemRow> UEquip_Mgr::GetQSItemForIndex(int32 _Idx)
{
	if (m_QuickSlotArr.IsEmpty())
	{
		return nullptr;
	}

	return m_QuickSlotArr[_Idx];
}

int32 UEquip_Mgr::GetNextArrayIndex()
{
	int32 idx = CurQuickSlotIdx;
	int32 iCount = 0;
	while (++idx)
	{
		if (iCount >= 5)
		{
			UE_LOG(LogTemp, Warning, TEXT("퀵슬롯에 등록된 아이템 없음"));
			return CurQuickSlotIdx;
		}

		if (idx >= m_QuickSlotArr.Num())
		{
			idx = 0;
		}

		if (m_QuickSlotArr[idx] == nullptr)
		{
			++iCount;
		}
		else
		{
			break;
		}
	}
	return idx;
}

bool UEquip_Mgr::QuickSlotValidForArr()
{
	for (int32 i = 0; i < m_QuickSlotArr.Num(); ++i)
	{
		if (m_QuickSlotArr[i] != nullptr)
		{
			return true;
		}
	}

	return false;
}

bool UEquip_Mgr::QuickSlotValidForIdx(int32 _Idx)
{
	if (m_QuickSlotArr[_Idx] != nullptr)
	{
		return true;
	}

	return false;
}

void UEquip_Mgr::DecreaseLowerSlotItem(int32 _Idx)
{
	TSharedPtr<FInvenItemRow> pItem = GetQSItemForIndex(_Idx);

	if (nullptr == pItem)
	{
		UE_LOG(LogTemp, Error, TEXT("해당하는 아이템 정보를 찾을 수 없음"));
		return;
	}

	if (pItem->Stack > 0)
	{
		--pItem->Stack;

		UE_LOG(LogTemp, Warning, TEXT("남은 개수 : %d"), pItem->Stack);

		// 개수가 0이 되면 퀵슬롯에서 해당 아이템을 없앤다.
		if (pItem->Stack <= 0)
		{	
			EEQUIP_SLOT Slot = ConvertIdxToQuickSlot(_Idx);
			UInventory_Mgr::GetInst(m_World)->SubGameItem(Slot, pItem->ID);
		}
		// 개수가 0이 아니면 퀵슬롯에서 해당 아이템의 갯수만 줄인다.
		else
		{
			RenewQuickSlotUI(_Idx);
			ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
			if ( !IsValid(GameMode) )
			{
				UE_LOG(LogTemp, Error, TEXT("DecreaseLowerSlotItem 게임모드 캐스팅 실패"));
				return;
			}
			UUI_EquipMain* EquipMainUI = GameMode->GetEquipUI();
			EquipMainUI->RenewEquipItemStack(pItem->EquipedSlot, pItem->Stack);
		}
	}


}

void UEquip_Mgr::SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("SetEquipSlotMap 게임모드 캐스팅 실패"));
		return;
	}

	if ( _InvenItem == nullptr)
	{
		m_EquipItemMap.Emplace(_Slot);
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
		//UE_LOG(LogTemp, Error, TEXT("조회할 슬롯에 아이템이 장비되어있지 않음"));
		return nullptr;
	}
	FGameItemInfo* pInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(pItemRow->ID);

	UItem_InvenData* pItemData = NewObject<UItem_InvenData>();
	pItemData->SetItemImgPath(pInfo->IconImgPath);
	pItemData->SetItemName(pInfo->ItemName);
	pItemData->SetItemDesc(pInfo->Description);
	pItemData->SetItemQnt(pItemRow->Stack);
	pItemData->SetPhysicAtkVal(pInfo->PhysicAtk);
	pItemData->SetPhysicDefVal(pInfo->PhysicDef);
	pItemData->SetMagicAtkVal(pInfo->MagicAtk);
	pItemData->SetMagicDefVal(pInfo->MagicDef);
	pItemData->SetRestoreHP(pInfo->Restore_HP);
	pItemData->SetRestoreMP(pInfo->Restore_MP);
	pItemData->SetRequireStr(pInfo->Require_Str);
	pItemData->SetRequireDex(pInfo->Require_Dex);
	pItemData->SetRequireInt(pInfo->Require_Int);
	pItemData->SetMaximumStack(pInfo->Maximum_Stack);
	pItemData->SetItemType(pInfo->Type);
	pItemData->SetEquiped(pItemRow->EquipedSlot);
	pItemData->SetItemID(pItemRow->ID);

	return pItemData;
}

void UEquip_Mgr::SetQuickSlotArray(FInvenItemRow* _InvenItem, int32 _Idx, bool _Unequip)
{
	if (_Unequip)
	{
		m_QuickSlotArr[_Idx] = nullptr;

		if (_Idx == CurQuickSlotIdx)
		{
			// 현재 퀵슬롯의 아이템을 장비해제 했을경우 다음 퀵슬롯으로 자리를 옮긴다.
			CurQuickSlotIdx = GetNextArrayIndex();
		}

		for (int32 i = 0; i < m_QuickSlotArr.Num(); ++i)
		{
			if (m_QuickSlotArr[i] == nullptr)
			{
				continue;
			}
			UE_LOG(LogTemp, Display, TEXT("QuickSlotArr[%d] 채워짐"), i);
		}
		return;
	}

	for (int32 i = 0; i < m_QuickSlotArr.Num(); ++i)
	{
		if ( m_QuickSlotArr[i] == nullptr)
		{
			continue;
		}
		if ( m_QuickSlotArr[i]->ID == _InvenItem->ID && m_QuickSlotArr[i]->EquipedSlot != _InvenItem->EquipedSlot)
		{
			m_QuickSlotArr[i] = nullptr;
			break;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("QuickSlotArr 최대 인덱스 : %d"), m_QuickSlotArr.Num());

	m_QuickSlotArr[_Idx] = MakeShareable(_InvenItem);

	for (int32 i = 0; i < m_QuickSlotArr.Num(); ++i)
	{
		if ( m_QuickSlotArr[i] == nullptr)
		{
			continue;
		}
		UE_LOG(LogTemp, Display, TEXT("QuickSlotArr[%d] 채워짐"), i);
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

void UEquip_Mgr::RenewNextQuickSlotUI(int32 _Idx)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("RenewNextQuickSlotUI 게임모드 캐스팅 실패"));
		return;
	}
	UUI_Base* MainUI = GameMode->GetMainHUD();

	MainUI->GetQuickSlotUI()->RenewLowerQuickSlot(_Idx);
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
