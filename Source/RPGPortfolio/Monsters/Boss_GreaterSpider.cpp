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

	m_AnimInst = Cast<UAnim_GreaterSpider>(GetMesh()->GetAnimInstance());
}

void ABoss_GreaterSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAtkTrace)
	{
		MeleeAttackHitCheck();
	}
}

void ABoss_GreaterSpider::ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType)
{
	float iDamage;

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

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(HitInfo.GetActor());
	// 플레이어가 가드중일 때
	if ( pPlayer->GetbToggleGuard() )
	{
		FVector vMonsterDir = GetActorForwardVector().GetSafeNormal();
		bool bBlocked = pPlayer->BlockEnemyAttack(iDamage, vMonsterDir);

		// 플레이어의 가드에 공격이 막힐 경우
		if ( bBlocked )
		{

			return;
		}
	}

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);

	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.Normal, HitInfo, GetController(), this, DamageTypeBase);

	/*TSoftObjectPtr<USoundBase> DmgSound = m_DataAssetInfo.LoadSynchronous()->GetMonSoundData(m_Type)->DmgSound_Normal;
	if ( IsValid(DmgSound.LoadSynchronous()) )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DmgSound.LoadSynchronous(), HitInfo.GetActor()->GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 타격사운드 로드 실패"));
	}*/
}

float ABoss_GreaterSpider::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if ( GetbIsDead() )
	{
		return 0.f;
	}
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 공격 모션중이 아닐 때도 조건에 추가해야함
	if ( DamageEvent.IsOfType(FPointDamageEvent::ClassID) )
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>( &DamageEvent );
		fPhysicsWeight = 1.f;
		GetMesh()->SetAllBodiesBelowSimulatePhysics(PointDamageEvent->HitInfo.BoneName, true);
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PointDamageEvent->HitInfo.BoneName, fPhysicsWeight);
		GetMesh()->AddImpulseToAllBodiesBelow(PointDamageEvent->ShotDirection * 500.f, PointDamageEvent->HitInfo.BoneName, true);

		HitReactDelegate.BindUObject(this, &ABoss_Base::StopBoneHitReaction, PointDamageEvent->HitInfo.BoneName);
		GetWorld()->GetTimerManager().SetTimer(HitReactTimer, HitReactDelegate, 0.01f, true, 0.1f);
	}

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

	// 경직 수치 최대로 채우면 스턴 상태에 빠트리고 행동트리 일시적으로 중지
	if ( StaggerGauge >= 100.f )
	{
		AAIController* pAIController = Cast<AAIController>(GetController());
		if (IsValid(pAIController))
		{
			if (pAIController->GetBlackboardComponent())
			{
				// 경직 상태
			}
		}
		StaggerGauge = 0.f;
	}

	TSoftObjectPtr<USoundBase> HitSound = m_DataAsset->GetSoundGSpider(EGreaterSpider_STATE::HIT).LoadSynchronous();
	if ( IsValid(HitSound.LoadSynchronous()) )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound.LoadSynchronous(), GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 피격사운드 로드 실패"));
	}

	return 0.0f;
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

			ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE);
			bAtkTrace = false;
		}
	}
}

void ABoss_GreaterSpider::PlayAttackMontage(EGreaterSpider_STATE _State)
{
	TSoftObjectPtr<UAnimMontage> pMontage = m_DataAsset->GetAnimGSpider(_State).LoadSynchronous();
	if (!IsValid(pMontage.LoadSynchronous()))
	{
		UE_LOG(LogTemp, Warning, TEXT("거미여왕 공격몽타주 로드 실패"));
		return;
	}
	m_AnimInst->Montage_Play(pMontage.LoadSynchronous());
}

void ABoss_GreaterSpider::MonsterDead(AController* EventInstigator)
{
	AAIController* pAIController = Cast<AAIController>(GetController());
	pAIController->GetBrainComponent()->StopLogic(TEXT("Dead"));

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(EventInstigator->GetPawn());

	TSoftObjectPtr<USoundBase> DeadSound = m_DataAsset->GetSoundGSpider(EGreaterSpider_STATE::DEAD).LoadSynchronous();
	if ( IsValid(DeadSound.LoadSynchronous()) )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound.LoadSynchronous(), GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 사망사운드 로드 실패"));
	}
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
