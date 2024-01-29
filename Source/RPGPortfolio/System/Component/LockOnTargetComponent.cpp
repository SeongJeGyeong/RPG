// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnTargetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

ULockOnTargetComponent::ULockOnTargetComponent()
{
	SetCollisionProfileName(FName("LOT"));

}