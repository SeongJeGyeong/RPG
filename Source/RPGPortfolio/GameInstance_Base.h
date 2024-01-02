// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstance_Base.generated.h"

class UPlayer_Menu_Mgr;

UCLASS()
class RPGPORTFOLIO_API UGameInstance_Base : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPlayer_Menu_Mgr* m_MenuMgr;

public:
	UGameInstance_Base();
	~UGameInstance_Base();

	// Player_Menu_Mgr에서 GameInstance의 private 멤버를 사용할 수 있음
	friend class UPlayer_Menu_Mgr;
};
