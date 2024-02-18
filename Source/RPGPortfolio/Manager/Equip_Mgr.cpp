// Fill out your copyright notice in the Description page of Project Settings.


#include "Equip_Mgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../GameInstance_Base.h"
#include "../UI/UI_Player_QuickSlot.h"
#include "../UI/UI_Base.h"

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

void UEquip_Mgr::SetEquickItemData(FInvenItemRow _InvenItem, EEQUIP_SLOT _Slot)
{

}

void UEquip_Mgr::SetEquickSlotArray(FInvenItemRow* _InvenItem, int32 _Idx)
{
	ARPGPortfolioGameModeBase* GameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
	if ( !IsValid(GameMode) )
	{
		UE_LOG(LogTemp, Error, TEXT("게임모드 캐스팅 실패"));
		return;
	}
	UUI_Base* MainUI = GameMode->GetMainHUD();

	if (_InvenItem == nullptr)
	{
		QuickSlotArr[_Idx] = _InvenItem;

		if (_Idx == CurQuickSlotIdx)
		{
			MainUI->GetQuickSlotUI()->RenewLowerQuickSlot(_Idx);
		}
		return;
	}

	for (int32 i = 0; i < QuickSlotArr.Num(); ++i)
	{
		if (QuickSlotArr[i] == nullptr)
		{
			continue;
		}
		if (QuickSlotArr[i]->ItemInfo->ID == _InvenItem->ItemInfo->ID && QuickSlotArr[i]->EquipedSlot != _InvenItem->EquipedSlot)
		{
			QuickSlotArr[i] = nullptr;
			break;
		}
	}

	QuickSlotArr[_Idx] = _InvenItem;

	if (_Idx == CurQuickSlotIdx)
	{
		MainUI->GetQuickSlotUI()->RenewLowerQuickSlot(_Idx);
	}
}

int32 UEquip_Mgr::ConvertSlotToIdx(EEQUIP_SLOT _Slot)
{
	int32 Index = -1;
	switch ( _Slot )
	{
	case EEQUIP_SLOT::WEAPON_1:
		break;
	case EEQUIP_SLOT::WEAPON_2:
		break;
	case EEQUIP_SLOT::WEAPON_3:
		break;
	case EEQUIP_SLOT::SHIELD_1:
		break;
	case EEQUIP_SLOT::SHIELD_2:
		break;
	case EEQUIP_SLOT::SHIELD_3:
		break;
	case EEQUIP_SLOT::ARROW:
		break;
	case EEQUIP_SLOT::BOLT:
		break;
	case EEQUIP_SLOT::HELM:
		break;
	case EEQUIP_SLOT::CHEST:
		break;
	case EEQUIP_SLOT::GAUNTLET:
		break;
	case EEQUIP_SLOT::LEGGINGS:
		break;
	case EEQUIP_SLOT::ACCESSORIE_1:
		break;
	case EEQUIP_SLOT::ACCESSORIE_2:
		break;
	case EEQUIP_SLOT::ACCESSORIE_3:
		break;
	case EEQUIP_SLOT::ACCESSORIE_4:
		break;
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
