// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Menu_Mgr.h"
#include "../GameInstance_Base.h"
#include "../UI/UI_Base.h"
#include "../RPGPortfolioGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UWorld* UPlayer_Menu_Mgr::m_World = nullptr;

UPlayer_Menu_Mgr* UPlayer_Menu_Mgr::GetInst(UWorld* _World)
{
    m_World = _World;

    return GetInst(m_World->GetGameInstance());
}

UPlayer_Menu_Mgr* UPlayer_Menu_Mgr::GetInst(UGameInstance* _GameInst)
{
    UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(_GameInst);

    if (!IsValid(pGameInst->m_MenuMgr))
    {
        pGameInst->m_MenuMgr = NewObject<UPlayer_Menu_Mgr>();
        pGameInst->m_MenuMgr->AddToRoot();
    }

    return pGameInst->m_MenuMgr;
}

void UPlayer_Menu_Mgr::ShowMenuUI(bool _Show)
{
    ARPGPortfolioGameModeBase* pGameMode = Cast<ARPGPortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
    if (!IsValid(pGameMode))
    {
        UE_LOG(LogTemp, Error, TEXT("GameMode Not Found"));
        return;
    }

    UUI_Base* MainUI = pGameMode->GetMainHUD();
    MainUI->ShowMenu(_Show);
}