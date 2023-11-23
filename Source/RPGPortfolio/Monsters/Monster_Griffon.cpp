// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Griffon.h"
#include "../MonsterAnim/Anim_Griffon.h"

AMonster_Griffon::AMonster_Griffon()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMonster_Griffon::BeginPlay()
{
	Super::BeginPlay();
}

void AMonster_Griffon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAtkTrace)
	{
		AttackHitCheck();
	}
}

void AMonster_Griffon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMonster_Griffon::AttackHitCheck()
{
	FVector vAtkCollisonStart;
	FVector vAtkCollisonEnd;

	switch (iAtkPattern)
	{
	case 1:
		vAtkCollisonStart = GetMesh()->GetSocketLocation("RightArm");
		vAtkCollisonEnd = GetMesh()->GetSocketLocation("GRIFFON_RightHand");
		break;
	case 2:
		vAtkCollisonStart = GetMesh()->GetSocketLocation("LeftArm");
		vAtkCollisonEnd = GetMesh()->GetSocketLocation("GRIFFON_LeftHand");
		break;
	case 3:
		vAtkCollisonStart = GetMesh()->GetSocketLocation("Head_Start");
		vAtkCollisonEnd = GetMesh()->GetSocketLocation("Head_Target");
		break;
	default:
		break;
	}

	float AtkRadius = 50.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		vAtkCollisonStart,
		vAtkCollisonEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeCapsule(AtkRadius, (vAtkCollisonEnd - vAtkCollisonStart).Size() * 0.5f),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;
	FVector vMidpoint = FMath::Lerp(vAtkCollisonEnd, vAtkCollisonStart, 0.5f);
	DrawDebugCapsule(GetWorld(), vMidpoint, (vAtkCollisonEnd - vAtkCollisonStart).Size() * 0.5f, AtkRadius, FRotationMatrix::MakeFromZ(vAtkCollisonEnd - vAtkCollisonStart).ToQuat(), color, false, 0.5f);

	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
			bAtkTrace = false;
		}
	}
}