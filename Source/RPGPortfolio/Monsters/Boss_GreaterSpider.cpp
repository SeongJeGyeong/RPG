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
#include "../GameInstance_Base.h"
#include "../Manager/GISubsystem_EffectMgr.h"
#include "../System/Subsys_ObjectPool.h"
#include "MotionWarpingComponent.h"

ABoss_GreaterSpider::ABoss_GreaterSpider()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	m_PSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	m_PSC->SetupAttachment(GetRootComponent());
	m_PSC->bAutoActivate = false;

	static ConstructorHelpers::FClassFinder<AProj_GS_Spiderling> projectile(TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Projectile/BPC_Proj_GS_Spiderling.BPC_Proj_GS_Spiderling_C'"));
	if ( projectile.Succeeded() )
	{
		m_GSProj = projectile.Class;
	}

	m_MWComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MWComponent"));
}

void ABoss_GreaterSpider::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);
}

void ABoss_GreaterSpider::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInst = Cast<UAnim_GreaterSpider>(GetMesh()->GetAnimInstance());
	if (IsValid(m_AnimInst))
	{
		m_AnimInst->OnRushAttack.AddUObject(this, &ABoss_GreaterSpider::RushAttack);
		m_AnimInst->OnRangedAttack.AddUObject(this, &ABoss_GreaterSpider::RangedAttack);
		m_AnimInst->OnSwingAttack.AddUObject(this, &ABoss_GreaterSpider::SwingAttack);
		m_AnimInst->OnStompAttack.AddUObject(this, &ABoss_GreaterSpider::StompAttack);
	}

	UGameInstance_Base* pGameInst = Cast<UGameInstance_Base>(GetGameInstance());
	pGameInst->ASyncLoadDataAsset(m_DataAsset.ToSoftObjectPath());

	if ( m_GSProj != nullptr )
	{
		USubsys_ObjectPool* PoolSubsystem = GetWorld()->GetSubsystem<USubsys_ObjectPool>();
		if (!IsValid(PoolSubsystem) || !PoolSubsystem->PreLoadObjFromPool<AProj_GS_Spiderling>(m_GSProj, 2, GetOwner()) )
		{
			UE_LOG(LogTemp, Error, TEXT("스킬 투사체 프리로드 실패"));
		}
	}

}

void ABoss_GreaterSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAtkTrace)
	{
		MeleeAttackHitCheck();
	}

	if (bRushMove)
	{
		AddMovementInput(GetActorForwardVector(), 1.f);
	}
	
}

void ABoss_GreaterSpider::PlayGSMontage(EGreaterSpider_STATE _State)
{
	m_State = _State;

	if ( _State == EGreaterSpider_STATE::RIGHTATTACK || _State == EGreaterSpider_STATE::LEFTATTACK )
	{
		AAIController* pAIController = Cast<AAIController>(GetController());
		if ( IsValid(pAIController) )
		{
			AActor* pTarget = Cast<AActor>(pAIController->GetBlackboardComponent()->GetValueAsObject(FName("Target")));
			FRotator TargetRot = FRotationMatrix::MakeFromX(( pTarget->GetActorLocation() - GetActorLocation() ).GetSafeNormal()).Rotator();
			TargetRot.Pitch = 0.f;
			TargetRot.Roll = 0.f;
			m_MWComponent->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("BossRotate"), FVector::ZeroVector, TargetRot);
		}
	}

	UAnimMontage* pAtkMontage = m_DataAsset.IsPending() ? m_DataAsset.LoadSynchronous()->GetAnimGSpider(_State) : m_DataAsset.Get()->GetAnimGSpider(_State);
	if (!IsValid(pAtkMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("거미여왕 공격몽타주 로드 실패"));
		return;
	}
	m_AnimInst->Montage_Play(pAtkMontage);
}

void ABoss_GreaterSpider::RushAttack(bool _Rush)
{
	SetActorTickEnabled(_Rush);
	bRushMove = _Rush;
	if ( _Rush )
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
		PlayGSSound(EGreaterSpider_STATE::RUSHATTACK);

		m_PSC->ToggleActive();
		bAtkTrace = true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		PlayGSSound(EGreaterSpider_STATE::BODYSLAM);

		UFXSystemAsset* BodySlamEffect = m_DataAsset.IsPending() ? m_DataAsset.LoadSynchronous()->GetEffectGSpider(EGreaterSpider_STATE::BODYSLAM) : m_DataAsset.Get()->GetEffectGSpider(EGreaterSpider_STATE::BODYSLAM);
		if (IsValid(BodySlamEffect))
		{
			UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(Cast<UParticleSystem>(BodySlamEffect), GetMesh(), FName("Root"), FVector(0.f, 0.f, 100.f), FRotator(0.f, 90.f, 0.f), FVector(2.f, 2.f, 2.f));
		}

		AttackSphereHitCheck(GetActorLocation(), 400.f);
		m_PSC->ToggleActive();
		bAtkTrace = false;

		HitActorArr.Empty();
	}
}

void ABoss_GreaterSpider::AttackSphereHitCheck(FVector _Location, float _Radius)
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		_Location,
		_Location,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(_Radius),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;

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

			for ( AActor* HitActor : HitActorArr )
			{
				if ( HitResult.GetActor() == HitActor )
				{
					return;
				}
			}
			HitActorArr.Add(HitResult.GetActor());

			ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE, m_State);
		}
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

	USubsys_ObjectPool* ObjectPool = GetWorld()->GetSubsystem<USubsys_ObjectPool>();
	if ( !IsValid(ObjectPool) )
	{
		UE_LOG(LogTemp, Error, TEXT("오브젝트 풀 가져오기 실패"));
		return;
	}
	AProj_GS_Spiderling* pProjectile = ObjectPool->SpawnObjFromPool<AProj_GS_Spiderling>(m_GSProj, ProjectileLocation, GetActorRotation(), this);
	if ( IsValid(pProjectile) )
	{
		pProjectile->SetProjDamage(EATTACK_TYPE::MAGIC_RANGE, m_Info.MagicAtk);

		APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		pProjectile->LaunchMotion(pPlayer->GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("투사체 로드 실패"));
	}
}

void ABoss_GreaterSpider::SwingAttack(EGreaterSpider_STATE _state)
{
	UFXSystemAsset* SwingEffect = m_DataAsset.IsPending() ? m_DataAsset.LoadSynchronous()->GetEffectGSpider(_state) : m_DataAsset.Get()->GetEffectGSpider(_state);
	if ( IsValid(SwingEffect) )
	{
		FVector Loc = GetActorLocation() + ( GetActorForwardVector() * 250.f );
		Loc.Z = GetMesh()->GetBoneLocation(FName("Root")).Z + 10.f;
		FRotator Rot = GetActorRotation();
		Rot.Yaw -= 90.f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cast<UParticleSystem>(SwingEffect), Loc, Rot, FVector(3.f, 3.f, 3.f));
		Loc.Z -= 50.f;
		AttackSphereHitCheck(Loc, 300.f);
	}
}

void ABoss_GreaterSpider::StompAttack()
{
	UFXSystemAsset* StompEffect = m_DataAsset.IsPending() ? m_DataAsset.LoadSynchronous()->GetEffectGSpider(EGreaterSpider_STATE::CENTERATTACK) : m_DataAsset.Get()->GetEffectGSpider(EGreaterSpider_STATE::CENTERATTACK);
	if ( IsValid(StompEffect) )
	{
		FVector Loc = GetActorLocation() + ( GetActorForwardVector() * 450.f );
		Loc.Z = GetMesh()->GetBoneLocation(FName("Root")).Z - 20.f;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cast<UParticleSystem>(StompEffect), Loc, GetActorRotation(), FVector(1.5f, 1.5f, 1.5f));

		AttackSphereHitCheck(Loc, 300.f);
	}
}

void ABoss_GreaterSpider::MeleeAttackHitCheck()
{
	switch ( m_State )
	{
	case EGreaterSpider_STATE::RIGHTATTACK:
		SweepAtkTrace(FName("Left_LoArm_Start"), FName("Left_Knee_Start"), 30.f);
		SweepAtkTrace(FName("Left_Knee_Start"), FName("Left_Knee_End"), 40.f);
		break;
	case EGreaterSpider_STATE::LEFTATTACK:
		SweepAtkTrace(FName("Right_LoArm_Start"), FName("Right_Knee_Start"), 30.f);
		SweepAtkTrace(FName("Right_Knee_Start"), FName("Right_Knee_End"), 40.f);
		break;
	case EGreaterSpider_STATE::CENTERATTACK:
		SweepAtkTrace(FName("Left_Knee_Start"), FName("Left_Knee_End"), 40.f);
		SweepAtkTrace(FName("Right_Knee_Start"), FName("Right_Knee_End"), 40.f);
		SweepAtkTrace(FName("HeadAttack_Start"), FName("HeadAttack_End"), 40.f);
		break;
	case EGreaterSpider_STATE::RUSHATTACK:
		AttackSphereHitCheck(GetActorLocation(), 250.f);
		break;
	default:
		break;
	}
}

void ABoss_GreaterSpider::SweepAtkTrace(FName _Start, FName _End, float _Radius)
{
	FVector vStart = GetMesh()->GetSocketLocation(_Start);
	FVector vEnd = GetMesh()->GetSocketLocation(_End);
	FVector vMidpoint = FMath::Lerp(vEnd, vStart, 0.5f);
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	float fTraceHalfHeight = ( vEnd - vStart ).Size() * 0.5;
	if ( PrevTraceLoc.IsZero() )
	{
		PrevTraceLoc = vMidpoint;
	}
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		vMidpoint,
		vMidpoint - 0.01f,			// 언리얼5의 버그로, 스윕의 시작지점과 끝지점이 동일하면 impactpoint가 잘못된 값을 반환한다.(정규화된 값?)
		FRotationMatrix::MakeFromZ(vEnd - vStart).ToQuat(),
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeCapsule(_Radius, fTraceHalfHeight),
		Params
	);
	PrevTraceLoc = vMidpoint;
	
	//FColor color;
	//bResult ? color = FColor::Red : color = FColor::Green;
	//DrawDebugCapsule(GetWorld(), vMidpoint, fTraceHalfHeight, _Radius, FRotationMatrix::MakeFromZ(vEnd - vStart).ToQuat(), color, false, 0.5f);

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
			if (!pPlayer->CanBeDamaged())
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
		}
	}
}

void ABoss_GreaterSpider::ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType, EGreaterSpider_STATE _AtkState)
{
	float iDamage = 0.f;
	EATTACK_WEIGHT Weight = EATTACK_WEIGHT::LIGHT;
	switch ( _AtkType )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		iDamage = m_Info.PhysicAtk;
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		iDamage = m_Info.MagicAtk;
		break;
	default:
		break;
	}
	if ( _AtkState == EGreaterSpider_STATE::RUSHATTACK )
	{
		iDamage = iDamage * 2.f;
		Weight = EATTACK_WEIGHT::HEAVY;
	}
	else if ( _AtkState == EGreaterSpider_STATE::CENTERATTACK )
	{
		Weight = EATTACK_WEIGHT::HEAVY;
	}


	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);
	DamageTypeBase.GetDefaultObject()->SetAtkWeight(Weight);	
	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.ImpactPoint, HitInfo, GetController(), this, DamageTypeBase);
}

float ABoss_GreaterSpider::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (GetbBossDead())
	{
		return 0.f;
	}
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UDamageType_Base* pDamageType = Cast<UDamageType_Base>(DamageEvent.DamageTypeClass->GetDefaultObject());
	// 받은 공격타입에 따라 몬스터의 방어력 설정
	float fMonsterDef = 0.f;
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
	OnDamagedBossHP.Broadcast(m_Info.CurHP / m_Info.MaxHP, FinalDamage);
	//GetBossWidget()->SetHPRatio(m_Info.CurHP / m_Info.MaxHP);
	//GetBossWidget()->DisplayDMG(FinalDamage);
	
	// 사망 시
	if ( m_Info.CurHP <= 0.f && GetController() )
	{
		MonsterDead(EventInstigator);
		return 0.f;
	}

	// 체력 50% 이하 시 2페이즈
	if (m_Info.CurHP <= m_Info.MaxHP / 2.f && !bPhase2)
	{
		AAIController* pAIController = Cast<AAIController>(GetController());
		if (IsValid(pAIController))
		{
			if (pAIController->GetBlackboardComponent())
			{
				pAIController->GetBlackboardComponent()->SetValueAsBool(FName("DoOnce"), true);
				pAIController->GetBlackboardComponent()->SetValueAsBool(FName("Phase2"), true);
				bPhase2 = true;
			}
		}
		// 페이즈2 기술 이펙트 세팅
		UFXSystemAsset* Particle = m_DataAsset.IsPending() ? m_DataAsset.LoadSynchronous()->GetEffectGSpider(EGreaterSpider_STATE::RUSHATTACK) : m_DataAsset.Get()->GetEffectGSpider(EGreaterSpider_STATE::RUSHATTACK);
		m_PSC->SetTemplate(Cast<UParticleSystem>(Particle));
	}

	// 공격 모션중이 아닐 때도 조건에 추가해야함
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>( &DamageEvent );
		fPhysicsWeight = 1.f;

		// 피격 이펙트 스폰
		UGameplayStatics::SpawnEmitterAttached(GETHITEFFECT, GetMesh(), PointDamageEvent->HitInfo.BoneName, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.5f), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
		// 피격 부위가 메쉬나 Pelvis가 아닐경우(피직스 에셋 오류 방지를 위해)
		if (!PointDamageEvent->HitInfo.BoneName.IsEqual(FName("Pelvis")) && !PointDamageEvent->HitInfo.BoneName.IsEqual(FName("None")))
		{
			// 본 흔들림 표현
			GetMesh()->SetAllBodiesBelowSimulatePhysics(PointDamageEvent->HitInfo.BoneName, true);
			GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PointDamageEvent->HitInfo.BoneName, fPhysicsWeight);
			GetMesh()->AddImpulseToAllBodiesBelow(PointDamageEvent->ShotDirection * 500.f, PointDamageEvent->HitInfo.BoneName, true);

			HitReactDelegate.BindUObject(this, &ABoss_Base::StopBoneHitReaction, PointDamageEvent->HitInfo.BoneName);
			GetWorld()->GetTimerManager().SetTimer(HitReactTimer, HitReactDelegate, 0.01f, true, 0.1f);
		}
	}

	PlayGSSound(EGreaterSpider_STATE::HIT);

	return 0.0f;
}

void ABoss_GreaterSpider::MonsterDead(AController* EventInstigator)
{
	AAIController* pAIController = Cast<AAIController>(GetController());
	pAIController->GetBrainComponent()->StopLogic(TEXT("Dead"));
	m_AnimInst->StopAllMontages(1.f);
	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(EventInstigator->GetPawn());
	PlayGSSound(EGreaterSpider_STATE::DEADHIT);
	PlayGSSound(EGreaterSpider_STATE::DEAD);
	m_AnimInst->SetDeadAnim();

	pPlayer->GainMonsterSoul(m_Info.Dropped_Soul);
	// 록온 상태일 경우 해제
	if ( pPlayer->GetbIsLockOn() )
	{
		pPlayer->BreakLockOn();
	}
	GetController()->UnPossess();

	TArray<TObjectPtr<USceneComponent>> AttachCompArr = GetMesh()->GetAttachChildren();
	if ( AttachCompArr.Num() <= 0 )
	{
		UE_LOG(LogTemp, Warning, TEXT("부착된 락온 컴포넌트 없음"));
	}
	else
	{
		for (TObjectPtr<USceneComponent> AttachComp : AttachCompArr)
		{
			AttachComp->DestroyComponent();
		}
	}

	Super::MonsterDead();
}

void ABoss_GreaterSpider::PlayGSSound(EGreaterSpider_STATE _state)
{
	USoundBase* Sound = m_DataAsset.IsPending() ? m_DataAsset.LoadSynchronous()->GetSoundGSpider(_state) : m_DataAsset.Get()->GetSoundGSpider(_state);
	if ( !IsValid(Sound) )
	{
		UE_LOG(LogTemp, Warning, TEXT("보스 사운드 로드 실패"));
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, GetActorLocation());
}

void ABoss_GreaterSpider::SpawnGSEffect(EGreaterSpider_STATE _state)
{
	UFXSystemAsset* Effect = m_DataAsset.IsPending() ? m_DataAsset.LoadSynchronous()->GetEffectGSpider(_state) : m_DataAsset.Get()->GetEffectGSpider(_state);
	if ( !IsValid(Effect) )
	{
		UE_LOG(LogTemp, Warning, TEXT("보스 이펙트 로드 실패"));
		return;
	}
	UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(Cast<UParticleSystem>(Effect), GetMesh(), FName("Root"), FVector(0.f, 0.f, 100.f), FRotator(0.f, 90.f, 0.f), FVector(2.f, 2.f, 2.f));
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