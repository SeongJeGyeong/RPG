// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemSelectMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Item/Item_InvenData.h"

void UUI_ItemSelectMenu::NativeConstruct()
{
	Super::NativeConstruct();

	m_Txt_Use = Cast<UTextBlock>(GetWidgetFromName(L"Txt_Use"));
	m_Btn_Use = Cast<UButton>(GetWidgetFromName(L"Btn_Use"));
	m_Btn_Drop = Cast<UButton>(GetWidgetFromName(L"Btn_Drop"));
	m_Btn_Discard = Cast<UButton>(GetWidgetFromName(L"Btn_Discard"));
	m_Btn_DropAll = Cast<UButton>(GetWidgetFromName(L"Btn_Drop_All"));
	m_Btn_DiscardAll = Cast<UButton>(GetWidgetFromName(L"Btn_Discard_All"));

	if (!IsValid(m_Txt_Use) || !IsValid(m_Btn_Use) || !IsValid(m_Btn_Drop) ||
		!IsValid(m_Btn_Discard) || !IsValid(m_Btn_DropAll) || !IsValid(m_Btn_DiscardAll))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 선택 메뉴 캐스팅 실패"));
	}
	else
	{
		m_Btn_Use->OnClicked.AddDynamic(this, &UUI_ItemSelectMenu::UseBtnClicked);
	}
}

void UUI_ItemSelectMenu::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_ItemSelectMenu::UseBtnClicked()
{
	if (IsValid(m_SelectedItemData))
	{
		UE_LOG(LogTemp, Warning, TEXT("데이터 전달 성공"));
		UE_LOG(LogTemp, Warning, L"%s", *m_SelectedItemData->GetItemName());
		UE_LOG(LogTemp, Warning, TEXT("%d"), m_SelectedItemData->GetItemQnt());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("데이터 전달 실패"));
	}

}
