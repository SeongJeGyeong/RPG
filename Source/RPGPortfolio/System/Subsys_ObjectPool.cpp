// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsys_ObjectPool.h"
#include "Interface/PoolableObj.h"
#include "../Characters/State/State_Sprint.h"
#include "../Characters/State/State_Hit.h"
#include "../Characters/State/State_Attack.h"
#include "../Characters/State/State_Idle.h"
#include "../Characters/State/State_Dodge.h"
#include "../Characters/State/State_Jump.h"
#include "../Characters/State/State_JumpAttack.h"
#include "../Characters/State/State_HeavyAttack.h"
#include "../Characters/State/State_UseItem.h"
#include "../Characters/State/State_UseSkill.h"
#include "../Characters/State/State_Action.h"
#include "../Characters/State/State_Guard.h"
#include "../Characters/State/State_GuardBreak.h"
#include "../Characters/State/State_AttackWait.h"

void USubsys_ObjectPool::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	StatePool.Add(EPlayerStateType::IDLE, MakeUnique<State_Idle>());
	StatePool.Add(EPlayerStateType::SPRINT, MakeUnique<State_Sprint>());
	StatePool.Add(EPlayerStateType::HIT, MakeUnique<State_Hit>());
	StatePool.Add(EPlayerStateType::ATTACK, MakeUnique<State_Attack>());
	StatePool.Add(EPlayerStateType::DODGE, MakeUnique<State_Dodge>());
	StatePool.Add(EPlayerStateType::JUMP, MakeUnique<State_Jump>());
	StatePool.Add(EPlayerStateType::JUMPATTACK, MakeUnique<State_JumpAttack>());
	StatePool.Add(EPlayerStateType::HEAVYATTACK, MakeUnique<State_HeavyAttack>());
	StatePool.Add(EPlayerStateType::USEITEM, MakeUnique<State_UseItem>());
	StatePool.Add(EPlayerStateType::USESKILL_1, MakeUnique<State_UseSkill>());
	StatePool.Add(EPlayerStateType::ACTION, MakeUnique<State_Action>());
	StatePool.Add(EPlayerStateType::GUARD, MakeUnique<State_Guard>());
	StatePool.Add(EPlayerStateType::GUARDBREAK, MakeUnique<State_GuardBreak>());
	StatePool.Add(EPlayerStateType::ATTACK_WAIT, MakeUnique<State_AttackWait>());
}

void USubsys_ObjectPool::Deinitialize()
{
	if ( !ObjPools.IsEmpty() )
	{
		ObjPools.Empty();
	}

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

TUniquePtr<StateMachine> USubsys_ObjectPool::GetStateFromPool(EPlayerStateType _Type)
{
	if ( TUniquePtr<StateMachine>* tempState = StatePool.Find(_Type) )
	{
		TUniquePtr<StateMachine> pState = MoveTemp(*tempState);
		StatePool.Remove(_Type);
		return pState;
	}

	return nullptr;
}

void USubsys_ObjectPool::ReturnStateToPool(EPlayerStateType _Type, StateMachine* _State)
{
	StatePool.Add(_Type, TUniquePtr<StateMachine>(_State));
}
