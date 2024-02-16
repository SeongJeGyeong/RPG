// Fill out your copyright notice in the Description page of Project Settings.


#include "Equip_Mgr.h"
#include "../GameInstance_Base.h"

UWorld* UEquip_Mgr::m_World = nullptr;

UEquip_Mgr* UEquip_Mgr::GetInst(UWorld* _World)
{
    m_World = _World;

    return GetInst(m_World->GetGameInstance());
}

UEquip_Mgr* UEquip_Mgr::GetInst(UGameInstance* _GameInst)
{
	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(_GameInst);

	if (!IsValid(pGameInst->m_EquipMgr))
	{
		pGameInst->m_EquipMgr = NewObject<UEquip_Mgr>();
		pGameInst->m_EquipMgr->AddToRoot();
	}

	return pGameInst->m_EquipMgr;
}
