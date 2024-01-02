// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Player_Main.h"
#include "Components/ListView.h"

void UUI_Player_Main::NativeConstruct()
{
	Super::NativeConstruct();

	m_ListView = Cast<UListView>(GetWidgetFromName(TEXT("ItemTileView")));

	if (!IsValid(m_ListView))
	{
		UE_LOG(LogTemp, Error, TEXT("ListView Not Found"));
	}
	else
	{

	}

}

void UUI_Player_Main::NativeTick(const FGeometry& _Geo, float _DeltaTime)
{
}
