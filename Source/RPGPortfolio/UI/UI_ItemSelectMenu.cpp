// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemSelectMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Item/Item_InvenData.h"
#include "../Manager/Inventory_Mgr.h"
#include "../Manager/Equip_Mgr.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Player_Base_Knight.h"

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

	if (IsValid(m_SelectedItemData))
	{
		if (m_SelectedItemData->GetItemType() != EITEM_TYPE::CONSUMABLE)
		{
			m_Txt_Use->SetColorAndOpacity(FLinearColor::FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
			m_Btn_Use->SetIsEnabled(false);
		}
	}

	Super::NativeConstruct();
}

void UUI_ItemSelectMenu::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_ItemSelectMenu::UseBtnClicked()
{
	if (IsValid(m_SelectedItemData))
	{
		APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		
		UInventory_Mgr::GetInst(GetGameInstance())->CloseInventoryUI();
		pPlayer->CloseMenuUI();

		pPlayer->UseItem(m_SelectedItemData->GetItemID(), m_SelectedItemData->GetEquiped());

		// 아이템 사용후 대기시간 on
		pPlayer->SetbItemDelay(true);
		pPlayer->ItemDelaytime(1.f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("인벤토리 앵커 데이터 전달 실패"));
	}

}
