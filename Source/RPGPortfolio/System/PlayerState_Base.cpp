// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState_Base.h"

APlayerState_Base::APlayerState_Base()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerState_Base::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerState_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}