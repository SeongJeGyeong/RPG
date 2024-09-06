// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GISubsystem_SoundMgr.generated.h"

#define GETMENUSOUND(SoundEnum)			GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>()->GetMenuSoundForMgr(SoundEnum)
#define GETMONSOUNDMAP(MonsterType)		GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>()->GetMonSoundMapForMgr(MonsterType)
#define GETBGM							GetGameInstance()->GetSubsystem<UGISubsystem_SoundMgr>()->GetBGMForMgr()

enum class EMenuSound : uint8;
enum class EMONSTER_TYPE : uint8;

struct FMonsterSoundAsset;

class UDA_MenuSound;
class UDA_MonsterSound;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UGISubsystem_SoundMgr : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSoftObjectPtr<UDA_MenuSound>		m_MenuSound;
	UPROPERTY()
	TSoftObjectPtr<UDA_MonsterSound>	m_MonsterSound;

	UPROPERTY()
	TSoftObjectPtr<USoundBase>			m_BGM;


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	USoundBase* GetMenuSoundForMgr(EMenuSound _SoundType) const;
	USoundBase* GetBGMForMgr() const;

	FMonsterSoundAsset* GetMonSoundMapForMgr(EMONSTER_TYPE _MonType);
};
