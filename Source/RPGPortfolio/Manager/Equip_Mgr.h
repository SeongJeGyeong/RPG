// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Equip_Mgr.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UEquip_Mgr : public UObject
{
	GENERATED_BODY()
	
private:
	static UWorld* m_World;

public:
	static UEquip_Mgr* GetInst(UWorld* _World);
	static UEquip_Mgr* GetInst(UGameInstance* _GameInst);

};
