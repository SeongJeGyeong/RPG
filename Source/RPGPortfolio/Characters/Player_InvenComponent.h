// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_InvenComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryOpenDelegate, bool);	// 인벤토리 오픈 델리게이트

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayer_InvenComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UGISubsystem_InvenMgr* m_InvenMgr;

public:
	FOnInventoryOpenDelegate OnInventoryOpen;

public:	
	// Sets default values for this component's properties
	UPlayer_InvenComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void ChangeQuickSlot();
	FInvenItemRow* GetQuickSlotItem();
	FGameItemInfo* GetItemInfo(EITEM_ID _Id);
	void DecreaseInventoryItem(EITEM_ID _Id, EEQUIP_SLOT _Slot);

	void CloseInventory();
};
