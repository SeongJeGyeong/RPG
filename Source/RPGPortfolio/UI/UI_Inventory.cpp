// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Inventory.h"
#include "Components/TileView.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "../Item/Item_InvenData.h"
#include "../System/PlayerState_Base.h"
#include "../Manager/Inventory_Mgr.h"
#include "PaperSprite.h"
#include "Styling/SlateBrush.h"
#include "UI_ItemTooltip.h"
#include "UI_PlayerStat.h"
#include "UI_InvenItem.h"
#include "../System/DataAsset/DA_MenuSound.h"

void UUI_Inventory::NativeConstruct()
{
	if ( !IsValid(m_ItemName) )
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 이름 위젯 찾지 못함"));
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_CategoryText))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 텍스트 위젯 찾지 못함"));
	}
	else
	{
		m_CategoryText->SetText(FText::FromString(L"전체아이템"));
	}
	if (!IsValid(m_Category_Img))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 이미지 위젯 찾지 못함"));
	}

	if (!IsValid(m_Btn_Cat_Left) || !IsValid(m_Btn_Cat_Right))
	{
		UE_LOG(LogTemp, Error, TEXT("카테고리 버튼 위젯 찾지 못함"));
	}
	else
	{
		m_Btn_Cat_Left->OnClicked.AddDynamic(this, &UUI_Inventory::LeftBtnClicked);
		m_Btn_Cat_Right->OnClicked.AddDynamic(this, &UUI_Inventory::RightBtnClicked);
	}

	if (!IsValid(m_ItemTileView))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 타일 뷰 찾지 못함"));
	}
	else
	{
		m_ItemTileView->OnItemIsHoveredChanged().AddUObject(this, &UUI_Inventory::OnTileHovered);
	}

	if (!IsValid(m_ItemTooltipUI))
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 툴팁 위젯 찾지 못함"));
	}
	else
	{
		m_ItemTooltipUI->SetVisibility(ESlateVisibility::Hidden);
	}

	if (!IsValid(m_PlayerStatUI))
	{
		UE_LOG(LogTemp, Error, TEXT("캐릭터스탯 위젯 찾지 못함"));
	}

	eCategory = EITEM_TYPE::ALL;
	SetCategoryUI(eCategory);

	m_Sound = LoadObject<UDA_MenuSound>(nullptr, TEXT("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'"));
	if ( !IsValid(m_Sound) )
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 사운드 로드 실패"));
	}

	Super::NativeConstruct();
}

void UUI_Inventory::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Inventory::OnTileHovered(UObject* _ItemData, bool _Hovered)
{
	if (_Hovered)
	{
		// 인벤토리에서는 메뉴앵커 표시 되도록
		UUI_InvenItem* ItemUI = Cast<UUI_InvenItem>(m_ItemTileView->GetEntryWidgetFromItem(_ItemData));
		ItemUI->SetAnchorActive(true);

		UItem_InvenData* pData = Cast<UItem_InvenData>(_ItemData);

		m_ItemName->SetText(FText::FromString(pData->GetItemName()));
		m_ItemName->SetVisibility(ESlateVisibility::Visible);

		m_ItemTooltipUI->SetTooltipUI(pData);
		m_ItemTooltipUI->SetVisibility(ESlateVisibility::Visible);

		PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_SELECT));
	}
	else
	{
		m_ItemName->SetVisibility(ESlateVisibility::Hidden);
		m_ItemTooltipUI->SetVisibility(ESlateVisibility::Hidden);
	}

}

void UUI_Inventory::LeftBtnClicked()
{
	if (eCategory == EITEM_TYPE::ALL)
	{
		return;
	}
	else
	{
		int8 iCategory = static_cast<int8>(eCategory);
		eCategory = static_cast<EITEM_TYPE>(--iCategory);
		UInventory_Mgr::GetInst(GetWorld())->RenewInventoryUI(eCategory);
		SetCategoryUI(eCategory);

		PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_CHANGE));
	}
}

void UUI_Inventory::RightBtnClicked()
{
	if ( eCategory == EITEM_TYPE::MISC)
	{
		return;
	}
	else
	{
		int8 iCategory = static_cast<int8>( eCategory );
		eCategory = static_cast<EITEM_TYPE>(++iCategory);
		UInventory_Mgr::GetInst(GetWorld())->RenewInventoryUI(eCategory);
		SetCategoryUI(eCategory);

		PlaySound(m_Sound->GetMenuSound(EMenuSound::MENU_CHANGE));
	}
}

void UUI_Inventory::AddInventoryItem(UObject* _ItemData)
{
	m_ItemTileView->AddItem(_ItemData);
}

void UUI_Inventory::Clear()
{
	if (IsValid(m_ItemTileView))
	{
		m_ItemTileView->ClearListItems();
	}
}

bool UUI_Inventory::IsInventoryOpened()
{
	if (this->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UUI_Inventory::SetStatUI(APlayerState* _PlayerState)
{
	m_PlayerStatUI->SetPlayerStatUI(Cast<APlayerState_Base>(_PlayerState));
}

void UUI_Inventory::SetCategoryUI(const EITEM_TYPE _Type)
{
	UPaperSprite* pSprite = nullptr;
	UTexture2D* Category_2DTex = nullptr;

	switch (_Type)
	{
	case EITEM_TYPE::ALL:
		m_CategoryText->SetText(FText::FromString(L"전체아이템"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_All.Inven_Category_All'"));
		break;
	case EITEM_TYPE::CONSUMABLE:
		m_CategoryText->SetText(FText::FromString(L"소비아이템"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Consumable.Inven_Category_Consumable'"));
		break;
	case EITEM_TYPE::WEAPON:
		m_CategoryText->SetText(FText::FromString(L"무기"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Weapon.Inven_Category_Weapon'"));
		break;
	case EITEM_TYPE::SHIELD:
		m_CategoryText->SetText(FText::FromString(L"방패"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Shield.Inven_Category_Shield'"));
		break;
	case EITEM_TYPE::ARM_HELM:
		m_CategoryText->SetText(FText::FromString(L"투구"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Helm.Inven_Category_Helm'"));
		break;
	case EITEM_TYPE::ARM_CHEST:
		m_CategoryText->SetText(FText::FromString(L"갑옷"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Chest.Inven_Category_Chest'"));
		break;
	case EITEM_TYPE::ARM_GAUNTLET:
		m_CategoryText->SetText(FText::FromString(L"장갑"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Gauntlet.Inven_Category_Gauntlet'"));
		break;
	case EITEM_TYPE::ARM_LEGGINGS:
		m_CategoryText->SetText(FText::FromString(L"각반"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Leggings.Inven_Category_Leggings'"));
		break;
	case EITEM_TYPE::ACCESSORIE:
		m_CategoryText->SetText(FText::FromString(L"악세사리"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Accessorie.Inven_Category_Accessorie'"));
		break;
	case EITEM_TYPE::ARROWS:
		m_CategoryText->SetText(FText::FromString(L"화살"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Arrows.Inven_Category_Arrows'"));
		break;
	case EITEM_TYPE::SPELL:
		m_CategoryText->SetText(FText::FromString(L"마법"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Spell.Inven_Category_Spell'"));
		break;
	case EITEM_TYPE::KEY:
		m_CategoryText->SetText(FText::FromString(L"중요아이템"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Keys.Inven_Category_Keys'"));
		break;
	case EITEM_TYPE::MISC:
		m_CategoryText->SetText(FText::FromString(L"기타아이템"));
		pSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/DSResource/UI/Inven_Category_Img_Sprite_14.Inven_Category_Img_Sprite_14'"));
		break;
	default:
		break;
	}

	// 스프라이트를 브러쉬로 바꾸기 위해 MakeBrushFromSprite가 필요함
	// PaperSpriteBlueprintLibrary 는 Paper2D API에 포함되어 있지 않음
	// MakeBrushFromSprite함수를 쓸 수 없기 때문에 해당 함수의 내용을 직접 가져와서 사용
	const FSlateAtlasData SpriteAtlasData = pSprite->GetSlateAtlasData();
	const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();
	FSlateBrush Brush;
	Brush.SetResourceObject(pSprite);
	Brush.ImageSize = FVector2D(80, 100);

	m_Category_Img->SetBrush(Brush);
	
	//m_Category_Img->SetBrushFromTexture(Category_2DTex);
}
