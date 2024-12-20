// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_InvenComponent.h"
#include "../Manager/GISubsystem_InvenMgr.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/UI_Base.h"
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

		pGameMode->GetMainHUD()->BindInvenComp(this);

		m_InvenMgr->RenewQuickSlotUI();
	}
}

void UPlayer_InvenComponent::ChangeQuickSlot()
{
	if ( IsValid(m_InvenMgr) )
	{
		int32 Idx = m_InvenMgr->GetNextValidIndex();
		m_InvenMgr->SetCurrentIndex(Idx);
		EEQUIP_SLOT Slot = m_InvenMgr->ConvertIdxToQuickSlot(Idx);
		m_InvenMgr->RenewQuickSlotUI();

		OnQSChangeAnim.Broadcast();
	}
}

FInvenItemRow* UPlayer_InvenComponent::GetQuickSlotItem()
{
	if ( IsValid(m_InvenMgr) )
	{
		FInvenItemRow* pItem = m_InvenMgr->GetCurrentQSItem();
		return pItem;
	}

	return nullptr;
}

FGameItemInfo* UPlayer_InvenComponent::GetInventoryItemInfo(EITEM_ID _Id)
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
			m_InvenMgr->RenewQuickSlotUI();
		}
	}
}

void UPlayer_InvenComponent::AcquireDroppedItem(EITEM_ID _Id, int32 _Stack, UTexture2D* _Img)
{
	FInvenItemRow* pItemRow = m_InvenMgr->AddGameItem(_Id, _Stack);
	FGameItemInfo* pItemInfo = m_InvenMgr->GetItemInfo(_Id);
	if ( pItemRow == nullptr || pItemInfo == nullptr )
	{
		return;
	}
	// 획득한 아이템 정보 메시지 UI로 표시
	OnAcquireItem.Broadcast(pItemInfo->ItemName, _Stack, _Img);

	// 퀵슬롯에 등록된 아이템이 추가되었을 경우 퀵슬롯의 아이템 개수를 갱신
	if ( pItemInfo->Type == EITEM_TYPE::CONSUMABLE && pItemRow->EquipedSlot != EEQUIP_SLOT::EMPTY )
	{
		m_InvenMgr->IncreaseEquipItemStack(pItemRow->EquipedSlot, _Stack);
		m_InvenMgr->RenewQuickSlotUI();
	}
}

void UPlayer_InvenComponent::ItemDelaytime(float _DelayPercent)
{
	GetOwner()->GetWorldTimerManager().ClearTimer(ItemDelayTimer);
	bItemDelay = true;
	OnQSDelay.Broadcast(true);
	OnQSDelayRate.Broadcast(1.f);
	fDelayRate = _DelayPercent;

	GetOwner()->GetWorldTimerManager().SetTimer(ItemDelayTimer, FTimerDelegate::CreateWeakLambda(this, [_DelayPercent, this]
		{
			fDelayRate -= 0.01f;
			float UIRate = FMath::Clamp(fDelayRate / _DelayPercent, 0.f, _DelayPercent);
			OnQSDelayRate.Broadcast(UIRate);
			if ( fDelayRate <= 0.f )
			{
				bItemDelay = false;
				OnQSDelay.Broadcast(false);
				GetOwner()->GetWorldTimerManager().ClearTimer(ItemDelayTimer);
			}
		})
	,0.01f, true);
}
