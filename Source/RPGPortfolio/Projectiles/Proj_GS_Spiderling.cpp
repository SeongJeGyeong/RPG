// Fill out your copyright notice in the Description page of Project Settings.


#include "Proj_GS_Spiderling.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../System/DamageType_Base.h"
#include "../Characters/Player_Base_Knight.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

AProj_GS_Spiderling::AProj_GS_Spiderling()
{
	PrimaryActorTick.bCanEverTick = false;

	m_Hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("HitBox"));
	m_Hitbox->SetCollisionProfileName("MonsterRangeAtk");
	SetRootComponent(m_Hitbox);
	m_Hitbox->SetEnableGravity(true);

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_Particle->SetupAttachment(m_Hitbox);
	fLifeTime = 0.f;
}

void AProj_GS_Spiderling::BeginPlay()
{
	Super::BeginPlay();

	m_Hitbox->OnComponentHit.AddDynamic(this, &AProj_GS_Spiderling::OnHitProj);
	/*if (!IsValid(m_Particle->Template))
	{
		m_Particle->SetTemplate(GetProjBaseParticle());
	}*/
}

void AProj_GS_Spiderling::LaunchMotion(FVector _TargetVec)
{
	m_ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	m_ProjectileMovement->ProjectileGravityScale = 1.f;
	FVector LaunchVel;
	// 타겟을 향해 포물선을 그리며 날아가도록 Velocity를 계산
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, LaunchVel, GetActorLocation(), _TargetVec);

	m_ProjectileMovement->Velocity = LaunchVel;
}

void AProj_GS_Spiderling::OnHitProj(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("HitProj"));
	APlayer_Base_Knight* pTarget = Cast<APlayer_Base_Knight>(OtherActor);
	if (!IsValid(pTarget))
	{
		FRotator HitRot = UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal);
		PlayHitEffect(false, Hit.ImpactPoint, HitRot);

		// 바닥에 맞았을 경우 장판 이펙트 범위에 들어왔을 시 데미지 입히도록
		FHitResult HitResult;
		FCollisionQueryParams Params(NAME_None, false, this);

		bool bResult = GetWorld()->SweepSingleByChannel
		(
			HitResult,
			Hit.ImpactPoint,
			Hit.ImpactPoint,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel6,
			FCollisionShape::MakeBox(FVector(200.f, 200.f, 20.f)),
			Params
		);

		FColor color;
		bResult ? color = FColor::Red : color = FColor::Green;

		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(200.f, 200.f, 20.f), color, false, 0.5f);

		if (bResult)
		{
			if ( HitResult.GetActor()->IsValidLowLevel() )
			{
				APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(HitResult.GetActor());

				// 무적 상태가 아닐 경우
				if (IsValid(pPlayer) && pPlayer->CanBeDamaged())
				{
					TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
					DamageTypeBase.GetDefaultObject()->SetAtkType(eAtkType);
					UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), fAtkDamage, Hit.Normal, Hit, GetOwner()->GetInstigatorController(), this, DamageTypeBase);
				}
			}
		}

		//Destroy();
		return;
	}
	PlayHitEffect(true, Hit.Location);

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(eAtkType);
	UGameplayStatics::ApplyPointDamage(Hit.GetActor(), fAtkDamage, Hit.Normal, Hit, GetOwner()->GetInstigatorController(), this, DamageTypeBase);
	//Destroy();
}