// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_UndeadAssassin.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Characters/Player_Base_Knight.h"

AMonster_UndeadAssassin::AMonster_UndeadAssassin()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	m_DetectRange = 800.f;
	m_AtkRange = 200.f;
}

void AMonster_UndeadAssassin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( bAtkTrace )
	{
		MeleeAttackHitCheck();
	}
}

void AMonster_UndeadAssassin::MonsterAttackMelee()
{
	GetMesh()->GetAnimInstance()->Montage_Play(m_AnimDA->GetMonsterMontage(EMON_MONTAGE::ATTACK));
}

void AMonster_UndeadAssassin::MeleeAttackHitCheck()
{
	float AtkRadius = 10.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector vHitStart = GetMesh()->GetSocketLocation("Socket_HitStart");
	FVector vHitEnd = GetMesh()->GetSocketLocation("Socket_HitEnd");
	float fTraceHalfHeight = ( vHitEnd - vHitStart ).Size() * 0.5;
	FVector vMidpoint = FMath::Lerp(vHitEnd, vHitStart, 0.5f);
	FQuat Rot = FRotationMatrix::MakeFromZ(vHitEnd - vHitStart).ToQuat();
	if ( PrevTraceLoc.IsZero() )
	{
		PrevTraceLoc = vMidpoint;
	}

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		PrevTraceLoc,
		vMidpoint,
		Rot,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeCapsule(AtkRadius, fTraceHalfHeight),
		Params
	);

	PrevTraceLoc = vMidpoint;

	/*if (bDebug)
	{
	 	FVector CapsuleTip = Rot.RotateVector(FVector(0, 0, fTraceHalfHeight));
		DrawDebugCapsule(GetWorld(), PrevTraceLoc, fTraceHalfHeight, AtkRadius, Rot, FColor::Green, false, 1.f);
		DrawDebugCapsule(GetWorld(), vMidpoint, fTraceHalfHeight, AtkRadius, Rot, FColor::Green, false, 1.f);
		DrawDebugLine(GetWorld(), PrevTraceLoc + CapsuleTip, vMidpoint + CapsuleTip, FColor::Green, false, 1.f);
		DrawDebugLine(GetWorld(), PrevTraceLoc - CapsuleTip, vMidpoint - CapsuleTip, FColor::Green, false, 1.f);

		if (bResult)
		{
			FVector const BlockingHitPoint = HitResult.Location;
			DrawDebugCapsule(GetWorld(), BlockingHitPoint, fTraceHalfHeight, AtkRadius, Rot, FColor::Red, false, 1.f);
		}
	}*/

	if ( bResult )
	{
		if ( HitResult.GetActor()->IsValidLowLevel() )
		{
			APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(HitResult.GetActor());

			if ( !IsValid(pPlayer) )
			{
				UE_LOG(LogTemp, Display, TEXT("타격 상대가 플레이어가 아님"));
				return;
			}

			// 무적 상태일 경우
			if ( !pPlayer->CanBeDamaged() )
			{
				return;
			}

			Super::ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE);
			bAtkTrace = false;
		}
	}
}
