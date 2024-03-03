// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Message_Item.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../Header/Struct.h"

void UUI_Message_Item::NativeConstruct()
{
	Super::NativeConstruct();

	m_ItemImg = Cast<UImage>(GetWidgetFromName(TEXT("ItemImg")));
	m_ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	m_ItemQnt = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemQnt")));
}

void UUI_Message_Item::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
	Super::NativeTick(_Geo, _DeltaTime);
}

void UUI_Message_Item::SetItemMessage(FString _ItemName, FString _ItemImgPath, int32 _Stack)
{
	FString ItemImgPath = _ItemImgPath;
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);
	m_ItemName->SetText(FText::FromString(_ItemName));
	m_ItemQnt->SetText(FText::FromString(FString::Printf(TEXT("%d"), _Stack)));
}