// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableObj.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolableObj : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGPORTFOLIO_API IPoolableObj
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnSpawnObjFromPool(bool _Activate) = 0;
	virtual void OnReturnObjToPool() = 0;
};
