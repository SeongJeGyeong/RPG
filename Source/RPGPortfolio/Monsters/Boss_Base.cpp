// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "AI/AIC_Monster_Base.h"
#include "Components/WidgetComponent.h"
#include "../System/DamageType_Base.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Player_Base_Knight.h"

// Sets default values
ABoss_Base::ABoss_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoss_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss_Base::MeleeAttackHitCheck()
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
			if (pPlayer->GetbInvincible())
			{
				return;
			}

			ApplyPointDamage(HitResult, EATTACK_TYPE::PHYSIC_MELEE);
			bAtkTrace = false;
		}
	}
}

void ABoss_Base::ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType)
{
	float iDamage;

	switch ( _AtkType )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		iDamage = PhysicAtk;
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		iDamage = MagicAtk;
		break;
	default:
		break;
	}

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(HitInfo.GetActor());
	// 플레이어가 가드중일 때
	if (pPlayer->GetbToggleGuard())
	{
		FVector vMonsterDir = GetActorForwardVector().GetSafeNormal();
		bool bBlocked = pPlayer->BlockEnemyAttack(iDamage, vMonsterDir);

		// 플레이어의 가드에 공격이 막힐 경우
		if ( bBlocked )
		{
			/*TSoftObjectPtr<USoundBase> BlockSound = m_DataAssetInfo.LoadSynchronous()->GetMonSoundData(m_Type)->BlockSound;
			if ( IsValid(BlockSound.LoadSynchronous()) )
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockSound.LoadSynchronous(), GetActorLocation());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("몬스터 블록사운드 로드 실패"));
			}*/

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

float ABoss_Base::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UDamageType_Base* pDamageType = Cast<UDamageType_Base>(DamageEvent.DamageTypeClass->GetDefaultObject());

	// 받은 공격타입에 따라 몬스터의 방어력 설정
	float fMonsterDef;
	switch ( pDamageType->GetAtkType() )
	{
	case EATTACK_TYPE::PHYSIC_MELEE:
	case EATTACK_TYPE::PHYSIC_RANGE:
		fMonsterDef = PhysicDef;
		break;
	case EATTACK_TYPE::MAGIC_MELEE:
	case EATTACK_TYPE::MAGIC_RANGE:
		fMonsterDef = MagicDef;
		break;
	default:
		break;
	}

	// 몬스터의 방어력만큼 데미지 감소 후 몬스터 hp바 위젯에 반영
	FinalDamage = FMath::Clamp(FinalDamage - fMonsterDef, 0.f, FinalDamage);
	CurHP = FMath::Clamp(CurHP - FinalDamage, 0.f, MaxHP);
	// m_MonsterWidget->SetHPRatio(CurHP / MaxHP);
	// m_MonsterWidget->DisplayDMG(FinalDamage);

	// 사망 시
	if ( CurHP <= 0.f && GetController() )
	{
		MonsterDead(DamageCauser);
		return 0.f;
	}

	// 경직 수치 최대로 채우면 스턴 상태에 빠트리고 행동트리 일시적으로 중지
	if ( StaggerGauge >= 100.f )
	{
		AAIController* pAIController = Cast<AAIController>(GetController());
		if ( IsValid(pAIController) )
		{
			if ( pAIController->GetBlackboardComponent() )
			{
				pAIController->GetBrainComponent()->StopLogic("Hit");
			}
		}
		StaggerGauge = 0.f;
	}

	/*TSoftObjectPtr<USoundBase> HitSound = m_DataAssetInfo.LoadSynchronous()->GetMonSoundData(m_Type)->HitSound_Normal;
	if ( IsValid(HitSound.LoadSynchronous()) )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound.LoadSynchronous(), GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 피격사운드 로드 실패"));
	}*/

	return 0.0f;
}

void ABoss_Base::MonsterDead(AActor* DamageCauser)
{
}

void ABoss_Base::SetbLockedOn(bool _LockedOn)
{
	m_LockOnMarker->SetVisibility(_LockedOn);
	bLockedOn = _LockedOn;
}