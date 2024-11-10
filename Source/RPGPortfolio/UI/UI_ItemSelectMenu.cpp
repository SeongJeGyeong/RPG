// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemSelectMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Item/Item_InvenData.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Player_Base_Knight.h"
#include "../Manager/GISubsystem_InvenMgr.h"

void UUI_ItemSelectMenu::NativeConstruct()
{
	if (!IsValid(m_Txt_Use) || !IsValid(m_Btn_Use) || !IsValid(m_Btn_Drop) ||
		!IsValid(m_Btn_Discard) || !IsValid(m_Btn_DropAll) || !IsValid(m_Btn_DiscardAll))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 선택 메뉴 캐스팅 실패"));
	}
	else
	{
		m_Btn_Use->OnClicked.AddDynamic(this, &UUI_ItemSelectMenu::UseBtnClicked);
		m_Btn_Drop->SetIsEnabled(false);
		m_Btn_Discard->SetIsEnabled(false);
		m_Btn_DropAll->SetIsEnabled(false);
		m_Btn_DiscardAll->SetIsEnabled(false);
	}

	if (bItemUseDelay)
	{
		m_Txt_Use->SetColorAndOpacity(FLinearColor::FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
		m_Btn_Use->SetIsEnabled(false);
	}

	if (m_ID != EITEM_ID::END)
	{
		FGameItemInfo* pInfo = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>()->GetItemInfo(m_ID);
		if (pInfo->Type != EITEM_TYPE::CONSUMABLE)
		{
			m_Txt_Use->SetColorAndOpacity(FLinearColor::FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
			m_Btn_Use->SetIsEnabled(false);
		}
	}

	Super::NativeConstruct();
}

void UUI_ItemSelectMenu::UseBtnClicked()
{
	FGameItemInfo* pInfo = GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>()->GetItemInfo(m_ID);
	if (pInfo->Type == EITEM_TYPE::CONSUMABLE)
	{
		APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		GetGameInstance()->GetSubsystem<UGISubsystem_InvenMgr>()->CloseInventoryUI();
		pPlayer->SetVisibilityMenuUI(false);
		pPlayer->UseItem(m_ID, m_Slot);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리 앵커 데이터 오류"));
	}

}
