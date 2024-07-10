// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Message_Item.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../Header/Struct.h"

void UUI_Message_Item::NativeConstruct()
{
	Super::NativeConstruct();

	if ( !IsValid(m_ItemImg) || !IsValid(m_ItemName) || !IsValid(m_ItemQnt) )
	{
		UE_LOG(LogTemp, Warning, TEXT("아이템 메시지박스 로드 실패"));
	}
}

void UUI_Message_Item::SetItemMessage(FString _ItemName, FString _ItemImgPath, int32 _Stack)
{
	FString ItemImgPath = _ItemImgPath;
	UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *ItemImgPath);
	m_ItemImg->SetBrushFromTexture(pTex2D);
	m_ItemName->SetText(FText::FromString(_ItemName));
	m_ItemQnt->SetText(FText::FromString(FString::Printf(TEXT("%d"), _Stack)));
}

void UUI_Message_Item::SetItemMessage(FString _ItemName, UTexture2D* _Texture, int32 _Stack)
{
	m_ItemImg->SetBrushFromTexture(_Texture);
	m_ItemName->SetText(FText::FromString(_ItemName));
	m_ItemQnt->SetText(FText::FromString(FString::Printf(TEXT("%d"), _Stack)));
}
