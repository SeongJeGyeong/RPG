// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_ItemTooltip.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"

void UUI_ItemTooltip::NativeConstruct()
{
	if (!IsValid(m_Sel_ItemName) || !IsValid(m_Sel_Category) || !IsValid(m_Sel_Sub_Category) || !IsValid(m_Sel_CurQnt_Inven) ||
		!IsValid(m_Sel_MaxQnt_Inven) || !IsValid(m_Sel_CurQnt_Storage) || !IsValid(m_Sel_MaxQnt_Storage) || !IsValid(m_PhysicAtk) ||
		!IsValid(m_PhysicDef) || !IsValid(m_MagicAtk) || !IsValid(m_MagicDef) || !IsValid(m_Restore_HP) || !IsValid(m_Restore_MP) || 
		!IsValid(m_Item_Desc) || !IsValid(m_Sel_Req_Str) || !IsValid(m_Sel_Req_Dex) || !IsValid(m_Sel_Req_Int) || !IsValid(m_Sel_ItemImg)
		)
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 툴팁 UI 로드 실패"));
	}

	Super::NativeConstruct();
}

void UUI_ItemTooltip::SetTooltipUI(UItem_InvenData* _InvenData)
{
	SetSelectedCategoryText(_InvenData->GetItemType());

	m_Sel_ItemName->SetText(FText::FromString(_InvenData->GetItemName()));
	m_Item_Desc->SetText(FText::FromString(_InvenData->GetItemDesc()));
	m_Sel_CurQnt_Inven->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetItemQnt())));
	m_Sel_MaxQnt_Inven->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetMaximumStack())));
	m_Sel_CurQnt_Storage->SetText(FText::FromString(L"0"));
	m_Sel_MaxQnt_Storage->SetText(FText::FromString(FString::Printf(TEXT("%d"), _InvenData->GetMaximumStack())));
	
	m_PhysicAtk->SetText(GetStatText(_InvenData->GetPhysicAtkVal()));
	m_PhysicDef->SetText(GetStatText(_InvenData->GetPhysicDefVal()));
	m_MagicAtk->SetText(GetStatText(_InvenData->GetMagicAtkVal()));
	m_MagicDef->SetText(GetStatText(_InvenData->GetMagicDefVal()));
	m_Restore_HP->SetText(GetStatText(_InvenData->GetRestoreHP()));
	m_Restore_MP->SetText(GetStatText(_InvenData->GetRestoreMP()));
	m_Sel_Req_Str->SetText(GetStatText(_InvenData->GetRequireStr()));
	m_Sel_Req_Dex->SetText(GetStatText(_InvenData->GetRequireDex()));
	m_Sel_Req_Int->SetText(GetStatText(_InvenData->GetRequireInt()));

	FString ItemImgPath = _InvenData->GetItemImgPath();
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_Sel_ItemImg->SetBrushFromTexture(pTex2D);
}

void UUI_ItemTooltip::SetTooltipUI(FGameItemInfo* _Info, uint32 _Stack)
{
	SetSelectedCategoryText(_Info->Type);

	m_Sel_ItemName->SetText(FText::FromString(_Info->ItemName));
	m_Item_Desc->SetText(FText::FromString(_Info->Description));
	m_Sel_CurQnt_Inven->SetText(FText::FromString(FString::Printf(TEXT("%d"), _Stack)));
	m_Sel_MaxQnt_Inven->SetText(FText::FromString(FString::Printf(TEXT("%d"), _Info->Maximum_Stack)));
	m_Sel_CurQnt_Storage->SetText(FText::FromString(L"0"));
	m_Sel_MaxQnt_Storage->SetText(FText::FromString(FString::Printf(TEXT("%d"), _Info->Maximum_Stack)));

	m_PhysicAtk->SetText(GetStatText(_Info->PhysicAtk));
	m_PhysicDef->SetText(GetStatText(_Info->PhysicDef));
	m_MagicAtk->SetText(GetStatText(_Info->MagicAtk));
	m_MagicDef->SetText(GetStatText(_Info->MagicDef));
	m_Restore_HP->SetText(GetStatText(_Info->Restore_HP));
	m_Restore_MP->SetText(GetStatText(_Info->Restore_MP));
	m_Sel_Req_Str->SetText(GetStatText(_Info->Require_Str));
	m_Sel_Req_Dex->SetText(GetStatText(_Info->Require_Dex));
	m_Sel_Req_Int->SetText(GetStatText(_Info->Require_Int));

	FString ItemImgPath = _Info->IconImgPath;
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_Sel_ItemImg->SetBrushFromTexture(pTex2D);
}

void UUI_ItemTooltip::SetSelectedCategoryText(const EITEM_TYPE& _Type)
{
	switch ( _Type )
	{
	case EITEM_TYPE::ACCESSORIE:
		m_Sel_Category->SetText(FText::FromString(L"악세사리"));
		break;
	case EITEM_TYPE::ARM_HELM:
		m_Sel_Category->SetText(FText::FromString(L"투구"));
		break;
	case EITEM_TYPE::ARM_CHEST:
		m_Sel_Category->SetText(FText::FromString(L"갑옷"));
		break;
	case EITEM_TYPE::ARM_GAUNTLET:
		m_Sel_Category->SetText(FText::FromString(L"장갑"));
		break;
	case EITEM_TYPE::ARM_LEGGINGS:
		m_Sel_Category->SetText(FText::FromString(L"각반"));
		break;
	case EITEM_TYPE::ARROWS:
		m_Sel_Category->SetText(FText::FromString(L"화살"));
		break;
	case EITEM_TYPE::CONSUMABLE:
		m_Sel_Category->SetText(FText::FromString(L"소비아이템"));
		break;
	case EITEM_TYPE::KEY:
		m_Sel_Category->SetText(FText::FromString(L"열쇠"));
		break;
	case EITEM_TYPE::MISC:
		m_Sel_Category->SetText(FText::FromString(L"기타"));
		break;
	case EITEM_TYPE::SPELL:
		m_Sel_Category->SetText(FText::FromString(L"주문"));
		break;
	case EITEM_TYPE::WEAPON:
		m_Sel_Category->SetText(FText::FromString(L"무기"));
		break;
	case EITEM_TYPE::SHIELD:
		m_Sel_Category->SetText(FText::FromString(L"방패"));
		break;
	default:
		break;
	};
}

FText UUI_ItemTooltip::GetStatText(const float& _StatVal)
{
	FText StatTxt = FText::FromString(L"-");
	if ( 0.f <= _StatVal )
	{
		StatTxt = FText::FromString(FString::Printf(TEXT("%d"), (int32)_StatVal));
	}

	return StatTxt;
}