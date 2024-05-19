// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Header/Struct.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ProjectileAsset.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UDA_ProjectileAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Assets", Meta = (DisplayName = "ProjectileData"))
	TMap<EProjectileType, FProjectileAssets> m_ProjData;

public:
	FProjectileAssets GetProjectileData(EProjectileType _ProjType) const {return *m_ProjData.Find(_ProjType);}

};
