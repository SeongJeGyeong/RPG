// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Mgr.h"
#include "../GameInstance_Base.h"
#include "../UI/UI_Inventory.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UWorld* UInventory_Mgr::m_World = nullptr;

UInventory_Mgr* UInventory_Mgr::GetInst(UWorld* _World)
{
	m_World = _World;

	return GetInst(m_World->GetGameInstance());
}

UInventory_Mgr* UInventory_Mgr::GetInst(UGameInstance* _GameInst)
{
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(_GameInst);

	if (!IsValid(pGameInst->m_InvenMgr))
	{
		pGameInst->m_InvenMgr = NewObject<UInventory_Mgr>();
		
		// AddToRoot : 객체를 루트에 추가함
		// 루트에 존재하는 객체는 가비지컬렉터가 자동으로 삭제하지 않기 때문에
		// 대신 사용하지 않을 때는 수동으로 삭제해줘야함
		pGameInst->m_InvenMgr->AddToRoot();
	}

	return pGameInst->m_InvenMgr;
}

UInventory_Mgr::UInventory_Mgr()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> InvenClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Player/Inventory/BPC_UI_Inventory.BPC_UI_Inventory_C'"));
	if ( InvenClass.Succeeded())
	{
		InvenWidgetClass = InvenClass.Class;
	}
}

void UInventory_Mgr::ShowInventoryUI()
{
	InvenUI = Cast<UUI_Inventory>(CreateWidget(m_World, InvenWidgetClass));
	if ( IsValid(InvenUI) )
	{
		InvenUI->AddToViewport(5);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("인벤토리 UI 생성 실패"));
	}
}
