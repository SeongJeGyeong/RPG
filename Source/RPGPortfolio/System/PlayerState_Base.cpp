// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Base.h"

APlayerState_Base::APlayerState_Base()
{
}

int32 APlayerState_Base::GetPlayerLevel() const
{
	return int32();
}

int32 APlayerState_Base::GetPlayerSoul() const
{
	return int32();
}

void APlayerState_Base::InitPlayerData(FCharacterStatSheet _PlayerStat)
{
}

void APlayerState_Base::SetPlayerStat(FCharacterStatSheet _PlayerStat)
{
}
