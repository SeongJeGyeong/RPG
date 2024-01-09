// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Dropped_Base.h"

// Sets default values
AItem_Dropped_Base::AItem_Dropped_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItem_Dropped_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem_Dropped_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem_Dropped_Base::BeginOverlap(UPrimitiveComponent* _PrimitiveCom, AActor* _OtherActor, UPrimitiveComponent* _OtherPrimitiveCom, int32 _Index, bool _bFromSweep, const FHitResult& _HitResult)
{

}

void AItem_Dropped_Base::OnNiagaraFinished(UNiagaraComponent* _Niagara)
{
	Destroy();
}

