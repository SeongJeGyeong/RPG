// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsys_ObjectPool.h"
#include "Interface/PoolableObj.h"

void USubsys_ObjectPool::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USubsys_ObjectPool::Deinitialize()
{
	Super::Deinitialize();
}

void USubsys_ObjectPool::ReturnObjToPool(AActor* _Poolable)
{
	if (_Poolable->GetClass()->ImplementsInterface(UPoolableObj::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("풀에 집어넣음"));
		TScriptInterface<IPoolableObj> PoolInterface = TScriptInterface<IPoolableObj>(_Poolable);
		PoolInterface->OnReturnObjToPool();
		FObjectPoolArr* ObjectPool = ObjPools.Find(_Poolable->GetClass());
		ObjectPool->Add(_Poolable);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("파괴함"));
		_Poolable->Destroy();
	}
}
