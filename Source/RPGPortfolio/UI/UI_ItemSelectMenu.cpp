// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemSelectMenu.h"
#include "Components/Button.h"

void UUI_ItemSelectMenu::NativeConstruct()
{
	Super::NativeConstruct();

	m_Btn_Use = Cast<UButton>(GetWidgetFromName(L"Btn_Use"));
	m_Btn_Drop = Cast<UButton>(GetWidgetFromName(L"Btn_Drop"));
	m_Btn_Discard = Cast<UButton>(GetWidgetFromName(L"Btn_Discard"));
	m_Btn_DropAll = Cast<UButton>(GetWidgetFromName(L"Btn_Drop_All"));
	m_Btn_DiscardAll = Cast<UButton>(GetWidgetFromName(L"Btn_Discard_All"));

	if (!IsValid(m_Btn_Use) || !IsValid(m_Btn_Drop) || !IsValid(m_Btn_Discard) ||
		!IsValid(m_Btn_DropAll) || !IsValid(m_Btn_DiscardAll))
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 선택 메뉴 캐스팅 실패"));
	}

}

void UUI_ItemSelectMenu::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}
