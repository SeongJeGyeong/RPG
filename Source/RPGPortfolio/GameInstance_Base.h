// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_Base.generated.h"

class UInventory_Mgr;
class UEquip_Mgr;

UCLASS()
class RPGPORTFOLIO_API UGameInstance_Base : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UInventory_Mgr*		m_InvenMgr;
	UEquip_Mgr*			m_EquipMgr;

public:
	UGameInstance_Base();
	~UGameInstance_Base();

	// Inventory_Mgr에서 GameInstance의 private 멤버를 사용할 수 있음
	friend class UInventory_Mgr;
	friend class UEquip_Mgr;
};
