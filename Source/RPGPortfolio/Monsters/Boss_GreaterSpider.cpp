// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_GreaterSpider.h"
#include "../System/DamageType_Base.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Player_Base_Knight.h"

ABoss_GreaterSpider::ABoss_GreaterSpider()
{
}

void ABoss_GreaterSpider::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);
}

void ABoss_GreaterSpider::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInst = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
}

void ABoss_GreaterSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAtkTrace)
	{
		MeleeAttackHitCheck();
	}
}

void ABoss_GreaterSpider::MeleeAttackHitCheck()
{
	float AtkRadius = 20.f;

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector vHitStart = GetMesh()->GetSocketLocation("Socket_HitStart");
	FVector vHitEnd = GetMesh()->GetSocketLocation("Socket_HitEnd");
	float fTraceHalfHeight = ( vHitEnd - vHitStart ).Size() * 0.5;

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		vHitStart,
		vHitEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeCapsule(AtkRadius, fTraceHalfHeight),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;

	FVector vMidpoint = FMath::Lerp(vHitEnd, vHitStart, 0.5f);
	DrawDebugCapsule(GetWorld(), vMidpoint, fTraceHalfHeight, AtkRadius, FRotationMatrix::MakeFromZ(vHitEnd - vHitStart).ToQuat(), color, false, 0.5f);

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
			if ( pPlayer->GetbInvincible() )
			{
				return;
			}

			Super::ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE);
			bAtkTrace = false;
		}
	}
}

void ABoss_GreaterSpider::PlayAttackMontage(EGreaterSpider_STATE _State)
{
	UAnimMontage* pMontage = m_DataAsset->GetAnimGSpider(_State).LoadSynchronous();
	m_AnimInst->Montage_Play(pMontage);
}
