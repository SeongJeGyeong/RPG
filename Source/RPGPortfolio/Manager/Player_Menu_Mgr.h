// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Player_Menu_Mgr.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UPlayer_Menu_Mgr : public UObject
{
	GENERATED_BODY()
	
private:
	static UWorld* m_World;

public:
	static UPlayer_Menu_Mgr* GetInst(UWorld* _World);

	UFUNCTION(BlueprintCallable)
	static UPlayer_Menu_Mgr* GetInst(UGameInstance* _GameInst);

	void ShowMenuUI();

public:
	bool bShow = false;

};
