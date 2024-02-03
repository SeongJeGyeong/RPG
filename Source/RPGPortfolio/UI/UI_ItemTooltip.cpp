// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemTooltip.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"

void UUI_ItemTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemName = Cast<UTextBlock>(GetWidgetFromName(L"Sel_ItemName"));
	m_Category = Cast<UTextBlock>(GetWidgetFromName(L"Sel_Category"));
	m_SubCategory = Cast<UTextBlock>(GetWidgetFromName(L"Sel_Sub_Category"));
	m_CurQnt_Inven = Cast<UTextBlock>(GetWidgetFromName(L"Sel_CurQnt_Inven"));
	m_MaxQnt_Inven = Cast<UTextBlock>(GetWidgetFromName(L"Sel_MaxQnt_Inven"));
	m_CurQnt_Storage = Cast<UTextBlock>(GetWidgetFromName(L"Sel_CurQnt_Storage"));
	m_MaxQnt_Storage = Cast<UTextBlock>(GetWidgetFromName(L"Sel_MaxQnt_Storage"));
	m_Atk = Cast<UTextBlock>(GetWidgetFromName(L"ATK"));
	m_Def = Cast<UTextBlock>(GetWidgetFromName(L"DEF"));
	m_Restore_HP = Cast<UTextBlock>(GetWidgetFromName(L"Restore_HP"));
	m_Restore_MP = Cast<UTextBlock>(GetWidgetFromName(L"Restore_MP"));
	m_Desc = Cast<UTextBlock>(GetWidgetFromName(L"Item_Desc"));
	m_Req_Str = Cast<UTextBlock>(GetWidgetFromName(L"Sel_Req_Str"));
	m_Req_Dex = Cast<UTextBlock>(GetWidgetFromName(L"Sel_Req_Dex"));
	m_Req_Int = Cast<UTextBlock>(GetWidgetFromName(L"Sel_Req_Int"));
	m_ItemImg = Cast<UImage>(GetWidgetFromName(L"Sel_ItemImg"));

	if (!IsValid(m_ItemName) || !IsValid(m_Category) || !IsValid(m_SubCategory) || !IsValid(m_CurQnt_Inven) ||
		!IsValid(m_MaxQnt_Inven) || !IsValid(m_CurQnt_Storage) || !IsValid(m_MaxQnt_Storage) || !IsValid(m_Atk) ||
		!IsValid(m_Def) || !IsValid(m_Restore_HP) || !IsValid(m_Restore_MP) || !IsValid(m_Desc) || 
		!IsValid(m_Req_Str) || !IsValid(m_Req_Dex) || !IsValid(m_Req_Int) || !IsValid(m_ItemImg))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 툴팁 UI 캐스팅 실패"));
	}

}

void UUI_ItemTooltip::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_ItemTooltip::SetTooltipUI(UItem_InvenData* _InvenData)
{
	switch ( _InvenData->GetItemType() )
	{
	case EITEM_TYPE::ACCESSORIE:
		m_Category->SetText(FText::FromString(L"악세사리"));
		break;
	case EITEM_TYPE::ARM_HELM:
		m_Category->SetText(FText::FromString(L"투구"));
		break;
	case EITEM_TYPE::ARM_CHEST:
		m_Category->SetText(FText::FromString(L"갑옷"));
		break;
	case EITEM_TYPE::ARM_GAUNTLET:
		m_Category->SetText(FText::FromString(L"장갑"));
		break;
	case EITEM_TYPE::ARM_LEGGINGS:
		m_Category->SetText(FText::FromString(L"각반"));
		break;
	case EITEM_TYPE::ARROWS:
		m_Category->SetText(FText::FromString(L"화살"));
		break;
	case EITEM_TYPE::CONSUMABLE:
		m_Category->SetText(FText::FromString(L"소비아이템"));
		break;
	case EITEM_TYPE::KEY:
		m_Category->SetText(FText::FromString(L"열쇠"));
		break;
	case EITEM_TYPE::MISC:
		m_Category->SetText(FText::FromString(L"기타"));
		break;
	case EITEM_TYPE::SPELL:
		m_Category->SetText(FText::FromString(L"주문"));
		break;
	case EITEM_TYPE::WEAPON:
		m_Category->SetText(FText::FromString(L"무기"));
		break;
	case EITEM_TYPE::SHIELD:
		m_Category->SetText(FText::FromString(L"방패"));
		break;
	default:
		break;
	};
	//m_SubCategory

	m_ItemName->SetText(FText::FromString(_InvenData->GetItemName()));
	m_Desc->SetText(FText::FromString(_InvenData->GetItemDesc()));
	m_CurQnt_Inven->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetItemQnt())));
	m_MaxQnt_Inven->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetMaximumStack())));
	m_CurQnt_Storage->SetText(FText::FromString(L"0"));
	m_MaxQnt_Storage->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetMaximumStack())));
	
	if (0 > _InvenData->GetAtkVal())
	{
		m_Atk->SetText(FText::FromString(L"-"));
	}
	else
	{
		m_Atk->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)_InvenData->GetAtkVal())));
	}
	
	if (0 > _InvenData->GetDefVal() )
	{
		m_Def->SetText(FText::FromString(L"-"));
	}
	else
	{
		m_Def->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)_InvenData->GetDefVal())));
	}
	
	if (0 > _InvenData->GetRestoreHP() )
	{
		m_Restore_HP->SetText(FText::FromString(L"-"));
	}
	else
	{
		m_Restore_HP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)_InvenData->GetRestoreHP())));
	}

	if (0 > _InvenData->GetRestoreMP() )
	{
		m_Restore_MP->SetText(FText::FromString(L"-"));
	}
	else
	{
		m_Restore_MP->SetText(FText::FromString(FString::Printf(TEXT("%d"), (int32)_InvenData->GetRestoreMP())));
	}

	if (0 > _InvenData->GetRequireStr() )
	{
		m_Req_Str->SetText(FText::FromString(L"-"));
	}
	else
	{
		m_Req_Str->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetRequireStr())));
	}

	if (0 > _InvenData->GetRequireDex() )
	{
		m_Req_Dex->SetText(FText::FromString(L"-"));
	}
	else
	{
		m_Req_Dex->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetRequireDex())));
	}

	if (0 > _InvenData->GetRequireInt())
	{
		m_Req_Int->SetText(FText::FromString(L"-"));
	}
	else
	{
		m_Req_Int->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetRequireInt())));
	}

	FString ItemImgPath = _InvenData->GetItemImgPath();
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);
}
