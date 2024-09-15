// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_MonAssetMgr.h"
#include "../System/DataAsset/PDA_MonsterAssets.h"
#include "../GameInstance_Base.h"

void UGISubsystem_MonAssetMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_MonsterDataAsset = FSoftObjectPath("/Script/RPGPortfolio.PDA_MonsterAssets'/Game/Blueprint/DataAsset/BPC_PDA_MonsterAssets.BPC_PDA_MonsterAssets'");
	m_MonsterDataAsset.ToSoftObjectPath().PostLoadPath(nullptr);

	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	pGameInst->ASyncLoadDataAsset(m_MonsterDataAsset.ToSoftObjectPath());
}

void UGISubsystem_MonAssetMgr::Deinitialize()
{
	Super::Deinitialize();
}

FMonsterAnimAsset UGISubsystem_MonAssetMgr::GetMonAnimAsset(EMONSTER_TYPE _Type)
{
	if ( m_MonsterDataAsset.IsPending() )
	{
		return *m_MonsterDataAsset.LoadSynchronous()->GetMonAnimData(_Type);
	}

	return *m_MonsterDataAsset.Get()->GetMonAnimData(_Type);
}
