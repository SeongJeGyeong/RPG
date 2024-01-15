// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance_Base.h"
#include "Manager/Inventory_Mgr.h"

UGameInstance_Base::UGameInstance_Base()
	: m_InvenMgr(nullptr)
{
	ConstructorHelpers::FObjectFinder<UDataTable> ItemTable(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_ItemInfo.DT_ItemInfo'"));
	if (ItemTable.Succeeded())
	{
		UInventory_Mgr::GetInst(this)->SetItemDataTable(ItemTable.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("아이템 데이터테이블 찾지 못함"));
	}
}

UGameInstance_Base::~UGameInstance_Base()
{
}
