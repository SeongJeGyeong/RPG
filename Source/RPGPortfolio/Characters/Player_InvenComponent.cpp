// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_InvenComponent.h"
#include "../Manager/GISubsystem_InvenMgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/UI_Base.h"
#include "../UI/UI_Player_QuickSlot.h"
#include "../UI/UI_Inventory.h"

UPlayer_InvenComponent::UPlayer_InvenComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayer_InvenComponent::BeginPlay()
{
	Super::BeginPlay();

	m_InvenMgr = GetOwner()->GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>();
	if ( !IsValid(m_InvenMgr) )
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayer_QuickSlotComp : 인벤토리 매니저 로드 실패"));
	}
	else
	{
		ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetOwner()));
		if ( !IsValid(pGameMode) )
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
			return;
		}

		UUI_Player_QuickSlot* QuickSlotUI = pGameMode->GetMainHUD()->GetQuickSlotUI();
		pGameMode->GetInventoryUI()->BindInvenComponent(this);

		int32 CurIdx = m_InvenMgr->GetCurrentIndex();
		FInvenItemRow* CurItemRow = m_InvenMgr->GetQSItemForIndex(CurIdx);
		if ( CurItemRow == nullptr )
		{
			QuickSlotUI->InitLowerQuickSlot(nullptr, nullptr);
		}
		else
		{
			UItem_InvenData* pCurInvenItem = m_InvenMgr->GetInvenDataToItemRow(*CurItemRow);
			int32 NextIdx = m_InvenMgr->GetNextValidIndex();
			if ( CurIdx == NextIdx )
			{
				QuickSlotUI->InitLowerQuickSlot(pCurInvenItem, nullptr);
			}
			else
			{
				FInvenItemRow* NextItemRow = m_InvenMgr->GetQSItemForIndex(NextIdx);
				UItem_InvenData* pNextInvenItem = m_InvenMgr->GetInvenDataToItemRow(*NextItemRow);
				QuickSlotUI->InitLowerQuickSlot(pCurInvenItem, pNextInvenItem);
			}
		}
	}
}

void UPlayer_InvenComponent::ChangeQuickSlot()
{
	if ( IsValid(m_InvenMgr) )
	{
		int32 Idx = m_InvenMgr->GetNextValidIndex();
		m_InvenMgr->SetCurrentIndex(Idx);
		EEQUIP_SLOT Slot = m_InvenMgr->ConvertIdxToQuickSlot(Idx);
		m_InvenMgr->RenewQuickSlotUI(Slot);
	}
}

FInvenItemRow* UPlayer_InvenComponent::GetQuickSlotItem()
{
	if ( IsValid(m_InvenMgr) )
	{
		int32 iCurIdx = m_InvenMgr->GetCurrentIndex();
		FInvenItemRow* pItem = m_InvenMgr->GetQSItemForIndex(iCurIdx);
		return pItem;
	}

	return nullptr;
}

FGameItemInfo* UPlayer_InvenComponent::GetItemInfo(EITEM_ID _Id)
{
	if ( IsValid(m_InvenMgr) )
	{
		FGameItemInfo* pItemInfo = m_InvenMgr->GetItemInfo(_Id);
		return pItemInfo;
	}

	return nullptr;
}

void UPlayer_InvenComponent::DecreaseInventoryItem(EITEM_ID _Id, EEQUIP_SLOT _Slot)
{
	if ( IsValid(m_InvenMgr) )
	{
		m_InvenMgr->DecreaseInventoryItem(_Id);
		// 퀵슬롯에 장착되어있을 경우 퀵슬롯과 인벤토리 둘 다 감소
		if ( _Slot != EEQUIP_SLOT::EMPTY )
		{
			m_InvenMgr->DecreaseLowerSlotItem(_Slot);
		}
	}
}

void UPlayer_InvenComponent::CloseInventory()
{
	OnInventoryOpen.Broadcast(false);
}
