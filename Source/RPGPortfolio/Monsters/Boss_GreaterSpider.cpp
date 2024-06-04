// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_GreaterSpider.h"
#include "../System/DamageType_Base.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Player_Base_Knight.h"
#include "../MonsterAnim/Anim_GreaterSpider.h"
#include "../UI/UI_Boss.h"
#include "AI/Boss/AIC_Boss_Base.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/ShapeComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Projectiles/Proj_GS_Spiderling.h"

ABoss_GreaterSpider::ABoss_GreaterSpider()
{
	m_PSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));

	m_PSC->SetupAttachment(GetRootComponent());
	m_PSC->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> RushParticle (TEXT("/Script/Engine.ParticleSystem'/Game/Blueprint/Monster/Effect/GreaterSpider/P_HeldCharge_Fire_Rush_2.P_HeldCharge_Fire_Rush_2'"));
	if (RushParticle.Succeeded())
	{
		m_PSC->SetTemplate(RushParticle.Object);
	}
}

void ABoss_GreaterSpider::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);
}

void ABoss_GreaterSpider::BeginPlay()
{
	Super::BeginPlay();

	AAIController* pAIController = Cast<AAIController>(GetController());
	if (IsValid(pAIController))
	{
		// 블랙보드에 몬스터정보 전달
		if (pAIController->GetBlackboardComponent())
		{
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("RangedAtkRange"), RangedAtkRange);
		}
	}

	m_AnimInst = Cast<UAnim_GreaterSpider>(GetMesh()->GetAnimInstance());

	if (IsValid(m_AnimInst))
	{
		m_AnimInst->OnRushAttack.AddUObject(this, &ABoss_GreaterSpider::RushAttack);
	}
}

void ABoss_GreaterSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAtkTrace)
	{
		fTraceInterval -= DeltaTime;
		MeleeAttackHitCheck();
	}
	/*if (bRushTrace)
	{
	}*/
	if (bRushMove)
	{
		AddMovementInput(GetActorForwardVector(), 1.f);
	}
	
}

void ABoss_GreaterSpider::PlayAttackMontage(EGreaterSpider_STATE _State)
{
	m_State = _State;

	UAnimMontage* pMontage = m_DataAsset->GetAnimGSpider(_State).LoadSynchronous();
	if ( !IsValid(pMontage) )
	{
		UE_LOG(LogTemp, Warning, TEXT("거미여왕 공격몽타주 로드 실패"));
		return;
	}
	m_AnimInst->Montage_Play(pMontage);
}

void ABoss_GreaterSpider::RushAttack(bool _Rush)
{
	bRushMove = _Rush;
	if ( _Rush )
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
		USoundBase* RushSound = m_DataAsset->GetSoundGSpider(EGreaterSpider_STATE::RUSHATTACK).LoadSynchronous();
		if ( IsValid(RushSound) )
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RushSound, GetActorLocation());
		}
		m_PSC->ToggleActive();
		bAtkTrace = true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		USoundBase* BodySlamSound = m_DataAsset->GetSoundGSpider(EGreaterSpider_STATE::BODYSLAM).LoadSynchronous();
		if ( IsValid(BodySlamSound) )
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BodySlamSound, GetActorLocation());
		}
		UParticleSystem* Particle = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Blueprint/Monster/Effect/GreaterSpider/P_HeldCharge_Fire_01_BodySlam.P_HeldCharge_Fire_01_BodySlam'"));
		UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(Particle, GetMesh(), FName("Root"), FVector(0.f, 0.f, 100.f), FRotator(0.f, 90.f, 0.f), FVector(2.f, 2.f, 2.f));

		m_PSC->ToggleActive();
		bAtkTrace = false;

		HitActorArr.Empty();
	}
}

void ABoss_GreaterSpider::RangedAttack()
{
	FActorSpawnParameters param = {};
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	param.OverrideLevel = GetLevel();
	param.bDeferConstruction = false;	// 지연생성(BeginPlay 호출 X)
	param.Owner = this;

	// 투사체 생성위치	
	FVector ProjectileLocation = GetMesh()->GetSocketLocation(FName("HeadAttack_End"));

	TSubclassOf<AProj_GS_Spiderling> ProjClass = LoadClass<AProj_GS_Spiderling>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Projectile/BPC_ShotSpiderling.BPC_ShotSpiderling_C'"));
	AProj_GS_Spiderling* pProjectile = GetWorld()->SpawnActor<AProj_GS_Spiderling>(ProjClass, ProjectileLocation, GetActorRotation(), param);
	pProjectile->SetProjDamage(EATTACK_TYPE::MAGIC_RANGE, m_Info.MagicAtk);
	pProjectile->SetShotInstigator(this);

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	pProjectile->LaunchMotion(pPlayer->GetActorLocation());
}

void ABoss_GreaterSpider::MeleeAttackHitCheck()
{
	switch ( m_State )
	{
	case EGreaterSpider_STATE::RIGHTATTACK:
		SweepAtkTrace(FName("Left_LoArm_Start"), FName("Left_Knee_Start"), 20.f);
		SweepAtkTrace(FName("Left_Knee_Start"), FName("Left_Knee_End"), 30.f);
		break;
	case EGreaterSpider_STATE::LEFTATTACK:
		SweepAtkTrace(FName("Right_LoArm_Start"), FName("Right_Knee_Start"), 20.f);
		SweepAtkTrace(FName("Right_Knee_Start"), FName("Right_Knee_End"), 30.f);
		break;
	case EGreaterSpider_STATE::CENTERATTACK:
		SweepAtkTrace(FName("Left_Knee_Start"), FName("Left_Knee_End"), 30.f);
		SweepAtkTrace(FName("Right_Knee_Start"), FName("Right_Knee_End"), 30.f);
		SweepAtkTrace(FName("HeadAttack_Start"), FName("HeadAttack_End"), 40.f);
		break;
	case EGreaterSpider_STATE::RUSHATTACK:
		if (fTraceInterval <= 0.f)
		{
			fTraceInterval = 0.1f;
			RushAttackHitCheck();
		}
		break;
	case EGreaterSpider_STATE::BODYSLAM:
		break;
	default:
		break;
	}
}

void ABoss_GreaterSpider::SweepAtkTrace(FName _Start, FName _End, float _Radius)
{
	FVector vStart = GetMesh()->GetSocketLocation(_Start);
	FVector vEnd = GetMesh()->GetSocketLocation(_End);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	float fTraceHalfHeight = ( vEnd - vStart ).Size() * 0.5;

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		vStart,
		vEnd,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeCapsule(_Radius, fTraceHalfHeight),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;

	FVector vMidpoint = FMath::Lerp(vEnd, vStart, 0.5f);
	DrawDebugCapsule(GetWorld(), vMidpoint, fTraceHalfHeight, _Radius, FRotationMatrix::MakeFromZ(vEnd - vStart).ToQuat(), color, false, 0.5f);

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

			for ( AActor* HitActor : HitActorArr )
			{
				if ( HitResult.GetActor() == HitActor )
				{
					return;
				}
			}
			HitActorArr.Add(HitResult.GetActor());

			ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE, m_State);
			//bAtkTrace = false;
		}
	}
}

void ABoss_GreaterSpider::RushAttackHitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(250.f),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;

	DrawDebugSphere(GetWorld(), GetActorLocation(), 200.f, 40, color, false, 0.4f);
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

			for ( AActor* HitActor : HitActorArr )
			{
				if ( HitResult.GetActor() == HitActor )
				{
					return;
				}
			}
			HitActorArr.Add(HitResult.GetActor());

			ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE, m_State);
			//bAtkTrace = false;
		}
	}
}

void ABoss_GreaterSpider::ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType, EGreaterSpider_STATE _AtkState)
{
	float iDamage;

	switch ( _AtkType )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		iDamage = m_Info.PhysicAtk;
		if (_AtkState == EGreaterSpider_STATE::RUSHATTACK)
		{
			iDamage = iDamage * 2.f;
		}
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		iDamage = m_Info.MagicAtk;
		break;
	default:
		break;
	}

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(HitInfo.GetActor());
	// 러시어택이 아니고 플레이어가 가드중일 때
	if (m_State != EGreaterSpider_STATE::RUSHATTACK && pPlayer->GetbToggleGuard())
	{	
		UE_LOG(LogTemp, Warning, TEXT("Guard"));
		FVector vMonsterDir = GetActorForwardVector().GetSafeNormal();
		bool bBlocked = pPlayer->BlockEnemyAttack(iDamage, -HitInfo.ImpactNormal.GetSafeNormal());
		if (bBlocked)
		{
			return;
		}
	}

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);

	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.Normal, HitInfo, GetController(), this, DamageTypeBase);
}

float ABoss_GreaterSpider::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if ( GetbIsDead() )
	{
		return 0.f;
	}
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UDamageType_Base* pDamageType = Cast<UDamageType_Base>(DamageEvent.DamageTypeClass->GetDefaultObject());
	// 받은 공격타입에 따라 몬스터의 방어력 설정
	float fMonsterDef;
	switch ( pDamageType->GetAtkType() )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		fMonsterDef = m_Info.PhysicDef;
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		fMonsterDef = m_Info.MagicDef;
		break;
	default:
		break;
	}

	// 몬스터의 방어력만큼 데미지 감소 후 몬스터 hp바 위젯에 반영
	FinalDamage = FMath::Clamp(FinalDamage - fMonsterDef, 0.f, FinalDamage);
	m_Info.CurHP = FMath::Clamp(m_Info.CurHP - FinalDamage, 0.f, m_Info.MaxHP);
	GetBossWidget()->SetHPRatio(m_Info.CurHP / m_Info.MaxHP);
	GetBossWidget()->DisplayDMG(FinalDamage);
	// 사망 시
	if ( m_Info.CurHP <= 0.f && GetController() )
	{
		MonsterDead(EventInstigator);
		return 0.f;
	}

	// 체력 50% 이하 시 2페이즈
	if (m_Info.CurHP <= m_Info.MaxHP / 2.f)
	{
		AAIController* pAIController = Cast<AAIController>(GetController());
		if (IsValid(pAIController))
		{
			if (pAIController->GetBlackboardComponent())
			{
				pAIController->GetBlackboardComponent()->SetValueAsBool(FName("Phase2"), true);
			}
		}
	}

	// 공격 모션중이 아닐 때도 조건에 추가해야함
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>( &DamageEvent );
		fPhysicsWeight = 1.f;

		// 피격 이펙트 스폰
		UParticleSystem* Particle = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
		UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(Particle, GetMesh(), PointDamageEvent->HitInfo.BoneName);

		// 피격 부위가 Plevis가 아닐경우(피직스 에셋 오류 방지를 위해)
		if (!PointDamageEvent->HitInfo.BoneName.IsEqual(FName("Plevis")))
		{
			// 본 흔들림 표현
			GetMesh()->SetAllBodiesBelowSimulatePhysics(PointDamageEvent->HitInfo.BoneName, true);
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PointDamageEvent->HitInfo.BoneName, fPhysicsWeight);
			GetMesh()->AddImpulseToAllBodiesBelow(PointDamageEvent->ShotDirection * 500.f, PointDamageEvent->HitInfo.BoneName, true);

			HitReactDelegate.BindUObject(this, &ABoss_Base::StopBoneHitReaction, PointDamageEvent->HitInfo.BoneName);
			GetWorld()->GetTimerManager().SetTimer(HitReactTimer, HitReactDelegate, 0.01f, true, 0.1f);
		}
	}

	// 경직 수치 최대로 채우면 스턴 상태에 빠트리고 행동트리 일시적으로 중지
	//if (StaggerGauge >= 100.f)
	//{
	//	AAIController* pAIController = Cast<AAIController>(GetController());
	//	if (IsValid(pAIController))
	//	{
	//		if (pAIController->GetBlackboardComponent())
	//		{
	//			// 경직 상태
	//		}
	//	}
	//	StaggerGauge = 0.f;
	//}

	USoundBase* HitSound = m_DataAsset->GetSoundGSpider(EGreaterSpider_STATE::HIT).LoadSynchronous();
	if (IsValid(HitSound))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 피격사운드 로드 실패"));
	}

	return 0.0f;
}

void ABoss_GreaterSpider::MonsterDead(AController* EventInstigator)
{
	AAIController* pAIController = Cast<AAIController>(GetController());
	pAIController->GetBrainComponent()->StopLogic(TEXT("Dead"));

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(EventInstigator->GetPawn());

	USoundBase* DeadSound = m_DataAsset->GetSoundGSpider(EGreaterSpider_STATE::DEAD).LoadSynchronous();
	if (!IsValid(DeadSound))
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 사망사운드 로드 실패"));
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound, GetActorLocation());
	m_AnimInst->SetDeadAnim();

	pPlayer->GainMonsterSoul(m_Info.Dropped_Soul);
	// 록온 상태일 경우 해제
	if ( pPlayer->GetbToggleLockOn() )
	{
		pPlayer->BreakLockOn();
	}
	GetController()->UnPossess();

	TArray<TObjectPtr<USceneComponent>> LockOnCompArr = GetMesh()->GetAttachChildren();
	if ( LockOnCompArr.Num() <= 0 )
	{
		UE_LOG(LogTemp, Warning, TEXT("부착된 락온 컴포넌트 없음"));
	}
	else
	{
		for ( TObjectPtr<USceneComponent> LockOnComp : LockOnCompArr )
		{
			// LockOnTarget 타입 Component일 경우
			if ( LockOnComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel1 )
			{
				LockOnComp->DestroyComponent();
			}
		}
	}
	Super::MonsterDead();
}

void ABoss_GreaterSpider::DeadCollisionSetting()
{
	TArray<USceneComponent*> ChildrenArr;
	GetRootComponent()->GetChildrenComponents(true, ChildrenArr);

	if (ChildrenArr.Num() > 0)
	{
		for (USceneComponent* ChildComp : ChildrenArr)
		{
			if (ChildComp->IsA(UShapeComponent::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("콜리전용 컴포넌트 찾음"));
				UShapeComponent* pCollsionComp = Cast<UShapeComponent>(ChildComp);
				pCollsionComp->SetCollisionProfileName(TEXT("IgnoreAll"));
			}
		}
	}
	GetMesh()->SetCollisionProfileName(TEXT("IgnoreAll"));
}