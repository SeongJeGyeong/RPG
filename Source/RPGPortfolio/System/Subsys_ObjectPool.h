// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsys_ObjectPool.generated.h"

USTRUCT()
struct FObjectPoolArr
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<AActor*> ObjectPool;

public:
	bool IsEmpty() const { return ObjectPool.IsEmpty(); }
	AActor* Pop() { return ObjectPool.Pop(); }
	void Add(AActor* _Actor) { ObjectPool.Add(_Actor); }
};

class AProjectile_Base;
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API USubsys_ObjectPool : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	/* TMap 안에 TArray를 넣으면 UPROPERTY 리플렉션을 달 수 없으므로
	구조체에 TArray 객체를 넣고 TMap에 구조체를 넣어 구현한다. */
	UPROPERTY()
	TMap<UClass*, FObjectPoolArr> ObjPools;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	template<typename T>
	bool PreLoadObjFromPool(TSubclassOf<AActor> _ObjClass, int32 _Quantity, AActor* _Owner = NULL);

	template<typename T>
	T* SpawnObjFromPool(TSubclassOf<AActor> _ObjClass, FVector _Loc, FRotator _Rot, AActor* _Owner = NULL);

	void ReturnObjToPool(AActor* _Poolable);

	/*UPROPERTY()
	TArray<AProjectile_Base*> ProjPool;*/
};

template<typename T>
bool USubsys_ObjectPool::PreLoadObjFromPool(TSubclassOf<AActor> _ObjClass, int32 _Quantity, AActor* _Owner)
{
	if ( !_ObjClass->ImplementsInterface(UPoolableObj::StaticClass()) )
	{
		return false;
	}

	for ( int32 i = 0; i < _Quantity; ++i )
	{
		FObjectPoolArr* PoolArr = &ObjPools.FindOrAdd(_ObjClass);

		FActorSpawnParameters param = {};
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		param.Owner = _Owner;
		T* PooledActor = GetWorld()->SpawnActor<T>(_ObjClass, FVector::ZeroVector, FRotator::ZeroRotator, param);
		if ( IsValid(PooledActor) )
		{
			PoolArr->Add(PooledActor);
			TScriptInterface<IPoolableObj> PoolInterface = TScriptInterface<IPoolableObj>(PooledActor);
			PoolInterface->OnSpawnObjFromPool(false);
		}
		else
		{
			return false;
		}
	}

	return true;
}

template<typename T>
T* USubsys_ObjectPool::SpawnObjFromPool(TSubclassOf<AActor> _ObjClass, FVector _Loc, FRotator _Rot, AActor* _Owner)
{
	T* PooledActor = nullptr;

	if ( _ObjClass.Get()->ImplementsInterface(UPoolableObj::StaticClass()) )
	{
		FObjectPoolArr& PoolArr = ObjPools.FindOrAdd(_ObjClass);
		if ( PoolArr.IsEmpty() )
		{
			FActorSpawnParameters param = {};
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			param.Owner = _Owner;
			PooledActor = GetWorld()->SpawnActor<T>(_ObjClass, _Loc, _Rot, param);
		}
		else
		{
			// CastChecked : ptrA = Cast< type >( ptrB ); check(ptrA != nullptr);
			PooledActor = CastChecked<T>(PoolArr.Pop());
			PooledActor->TeleportTo(_Loc, _Rot);
		}

		TScriptInterface<IPoolableObj> PoolInterface = TScriptInterface<IPoolableObj>(PooledActor);
		PoolInterface->OnSpawnObjFromPool(true);
	}

	return PooledActor;
}