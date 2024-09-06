// Fill out your copyright notice in the Description page of Project Settings.


#include "GISubsystem_SoundMgr.h"
#include "../System/DataAsset/DA_MenuSound.h"
#include "../System/DataAsset/DA_MonsterSound.h"
#include "../GameInstance_Base.h"
#include "../Header/Enum.h"

void UGISubsystem_SoundMgr::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_MenuSound = FSoftObjectPath("/Script/RPGPortfolio.DA_MenuSound'/Game/Blueprint/DataAsset/BPC_DA_MenuSound.BPC_DA_MenuSound'");
	m_MenuSound.ToSoftObjectPath().PostLoadPath(nullptr);

	m_MonsterSound = FSoftObjectPath("/Script/RPGPortfolio.DA_MonsterSound'/Game/Blueprint/DataAsset/BPC_DA_MonsterSound.BPC_DA_MonsterSound'");
	m_MonsterSound.ToSoftObjectPath().PostLoadPath(nullptr);

	m_BGM = FSoftObjectPath("/Script/Engine.SoundCue'/Game/Blueprint/Monster/Sound/GreaterSpider/SC_GS_BGM.SC_GS_BGM'");
	m_BGM.ToSoftObjectPath().PostLoadPath(nullptr);

	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	pGameInst->ASyncLoadDataAsset(m_MenuSound.ToSoftObjectPath());
	pGameInst->ASyncLoadDataAsset(m_MonsterSound.ToSoftObjectPath());
	pGameInst->ASyncLoadDataAsset(m_BGM.ToSoftObjectPath());
}

void UGISubsystem_SoundMgr::Deinitialize()
{
	Super::Deinitialize();
}

USoundBase* UGISubsystem_SoundMgr::GetMenuSoundForMgr(EMenuSound _SoundType) const
{
	if ( m_MenuSound.IsPending() )
	{
		return m_MenuSound.LoadSynchronous()->GetMenuSound(_SoundType);
	}

	return m_MenuSound.Get()->GetMenuSound(_SoundType);
}

USoundBase* UGISubsystem_SoundMgr::GetBGMForMgr() const
{
	if ( m_BGM.IsPending() )
	{
		return m_BGM.LoadSynchronous();
	}

	return m_BGM.Get();
}

FMonsterSoundAsset* UGISubsystem_SoundMgr::GetMonSoundMapForMgr(EMONSTER_TYPE _MonType)
{
	if ( m_MonsterSound.IsPending() )
	{
		return m_MonsterSound.LoadSynchronous()->GetMonsterSound(_MonType);
	}

	return m_MonsterSound.Get()->GetMonsterSound(_MonType);
}
