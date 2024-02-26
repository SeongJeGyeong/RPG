// Fill out your copyright notice in the Description page of Project Settings.


#include "Equip_Mgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "../System/PlayerState_Base.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"
#include "../UI/UI_Player_QuickSlot.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_StatusMain.h"

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

FInvenItemRow* UEquip_Mgr::GetSlotForIndex(int32 _Idx)
{
	if (m_QuickSlotArr.IsEmpty())
	{
		return nullptr;
	}

	return m_QuickSlotArr[_Idx];
}

int32 UEquip_Mgr::GetNextArrayIndex()
{
	while (++CurQuickSlotIdx)
	{
		if ( CurQuickSlotIdx >= m_QuickSlotArr.Num() )
		{
			CurQuickSlotIdx = 0;
		}
		if (m_QuickSlotArr[CurQuickSlotIdx] != nullptr)
		{
			break;
		}
	}

	return CurQuickSlotIdx;
}

bool UEquip_Mgr::GetQuickSlotValid()
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

void UEquip_Mgr::SetEquipSlotMap(FInvenItemRow* _InvenItem, EEQUIP_SLOT _Slot)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}

	if ( _InvenItem == nullptr)
	{
		m_EquipItemMap.Emplace(_Slot);
	}
	else
	{
		FGameItemInfo* pItemInfo = _InvenItem->ItemInfo;
		m_EquipItemMap.Emplace(_Slot, *pItemInfo);
	}

	UUI_StatusMain* StatusUI = GameMode->GetStatusUI();
	StatusUI->RenewStatusUI();

	//FGameItemInfo* pItemInfo = m_EquipItemMap.Find(_Slot);

	//if (pItemInfo == nullptr)
	//{
	//	m_EquipItemMap.Emplace(_Slot, _InvenItem);
	//}
	//else
	//{
	//	if (_InvenItem == nullptr)
	//	{
	//		m_EquipItemMap.Emplace(_Slot, _InvenItem);
	//	}
	//}
}

void UEquip_Mgr::SetQuickSlotArray(FInvenItemRow* _InvenItem, int32 _Idx, bool _Unequip)
{
	if (_Unequip)
	{
		//m_QuickSlotArr.Remove(_InvenItem);
		m_QuickSlotArr[_Idx] = nullptr;

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
		if ( m_QuickSlotArr[i]->ItemInfo->ID == _InvenItem->ItemInfo->ID && m_QuickSlotArr[i]->EquipedSlot != _InvenItem->EquipedSlot)
		{
			m_QuickSlotArr[i] = nullptr;
			//m_QuickSlotArr.RemoveAt(i);
			break;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("QuickSlotArr 최대 인덱스 : %d"), m_QuickSlotArr.Num());

	m_QuickSlotArr[_Idx] = _InvenItem;

	//// 인덱스가 배열의 Num()값 이상이면 add
	//// 인덱스가 배열의 범위 내면 Insert
	//if ( m_QuickSlotArr.IsValidIndex(_Idx) )
	//{
	//	m_QuickSlotArr[ _Idx ] = _InvenItem;
	//	//m_QuickSlotArr.Insert(_InvenItem, _Idx);
	//}
	//else
	//{
	//	m_QuickSlotArr.Add(_InvenItem);
	//}

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
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
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
