// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Base.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "AI/AIC_Monster_Base.h"
#include "Components/WidgetComponent.h"
#include "../UI/UI_Monster.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Player_Base_Knight.h"
#include "../MonsterAnim/AnimInstance_Monster_Base.h"
#include "../Item/Item_Dropped_Base.h"
#include "../System/DamageType_Base.h"
#include "Engine/DamageEvents.h"

// Sets default values
AMonster_Base::AMonster_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIC_Monster_Base::StaticClass();

	// widgetComponent
	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	if (!IsValid(m_WidgetComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetComponent Create Failed"));
	}
	m_WidgetComponent->SetupAttachment(GetRootComponent());
	m_WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	m_WidgetComponent->SetDrawSize(FVector2D(200.f, 200.f));

	ConstructorHelpers::FClassFinder<UUserWidget> MonsterUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Monster/BPC_UI_MonsterMain.BPC_UI_MonsterMain_C'"));
	if ( MonsterUI.Succeeded() )
	{
		m_WidgetComponent->SetWidgetClass(MonsterUI.Class);
	}

	ConstructorHelpers::FObjectFinder<UDataTable> ItemDropTable(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_MonsterDropTable.DT_MonsterDropTable'"));
	if (ItemDropTable.Succeeded())
	{
		m_ItemTable = ItemDropTable.Object;
	}
}

void AMonster_Base::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);

	FMonsterInfo* pInfo;
	 
	if (IsValid(m_MonsterInfoTableRow.DataTable) && !m_MonsterInfoTableRow.RowName.IsNone())
	{
		pInfo = m_MonsterInfoTableRow.DataTable->FindRow<FMonsterInfo>(m_MonsterInfoTableRow.RowName, TEXT(""));
		
		if (nullptr != pInfo)
		{
			m_Info = *pInfo;
		}
	}

	FString str;
	TArray<FMonsterItemDropTable*> DropTableArr;
	m_ItemTable->GetAllRows(str, DropTableArr);

	for (int32 i = 0; i < DropTableArr.Num(); ++i)
	{
		if (DropTableArr[ i ]->Monster == m_Type)
		{
			m_DropItemArr.Add(*DropTableArr[i]);
		}
	}
}

// Called when the game starts or when spawned
void AMonster_Base::BeginPlay()
{
	Super::BeginPlay();
	
	AAIController* pAIController = Cast<AAIController>(GetController());

	if (IsValid(pAIController))
	{
		// 블랙보드에 몬스터정보 전달
		if (pAIController->GetBlackboardComponent())
		{
			pAIController->GetBlackboardComponent()->SetValueAsVector(FName("SpawnPosition"), GetActorLocation());
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AtkRange"), m_Info.AtkRange);
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectRange"), m_Info.DetectRange);
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("PerceiveRange"), m_Info.BOSS_PerceiveRange);
		}
		m_AnimInst = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
		m_AnimInst->OnMontageEnded.AddDynamic(this, &AMonster_Base::OnHitMontageEnded);
	}
	
	m_MonsterWidget = Cast<UUI_Monster>(m_WidgetComponent->GetWidget());
	if (!IsValid(m_MonsterWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterWidget Casting Failed"));
	}
	else
	{
		m_MonsterWidget->SetName(m_Info.Name);
		m_MonsterWidget->SetHPRatio(1.f);
		m_Info.CurHP = m_Info.MaxHP;
	}
	m_WidgetComponent->SetVisibility(false);
}

// Called every frame
void AMonster_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAtkTrace)
	{
		MeleeAttackHitCheck();
	}
}

float AMonster_Base::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_State == EMONSTER_STATE::DEAD)
	{
		return 0.0f;
	}
	bAtkTrace = false;

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UDamageType_Base* pDamageType = Cast<UDamageType_Base>(DamageEvent.DamageTypeClass->GetDefaultObject());
	// 받은 공격타입에 따라 몬스터의 방어력 설정
	float fMonsterDef;
	switch ( pDamageType->GetAtkType())
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
	m_MonsterWidget->SetHPRatio(m_Info.CurHP / m_Info.MaxHP);
	m_WidgetComponent->SetVisibility(true);
	m_MonsterWidget->DisplayDMG(FinalDamage);

	// 피격 시 모든 애니메이션 중지
	UAnimInstance_Monster_Base* pAnimInst = Cast<UAnimInstance_Monster_Base>(GetMesh()->GetAnimInstance());
	pAnimInst->Montage_Stop(1.f);

	// 사망 시
	if ( m_Info.CurHP <= 0.f && GetController() )
	{
		MonsterDead(EventInstigator);
		return 0.f;
	}

	// 피격 이펙트 스폰
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		UParticleSystem* Particle = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
		UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAttached(Particle, GetMesh(), PointDamageEvent->HitInfo.BoneName);
	}

	if (!bLockedOn)
	{
		GetWorld()->GetTimerManager().ClearTimer(WidgetDisplayTimer);
		// 락온되지 않은 상태에서 3초 동안 맞지 않았을 경우 위젯 사라지도록
		GetWorld()->GetTimerManager().SetTimer(WidgetDisplayTimer, [this]() {m_WidgetComponent->SetVisibility(true); }, 0.1f, false, 3.f);
	}

	// 비헤이비어 트리 피격 상태로 전환
	AAIController* pAIController = Cast<AAIController>(GetController());
	if (IsValid(pAIController))
	{
		if ( pAIController->GetBlackboardComponent() )
		{
			m_State = EMONSTER_STATE::IDLE;
			pAIController->GetBlackboardComponent()->SetValueAsBool(FName("bHitted"), true);
			if (!IsValid(pAIController->GetBlackboardComponent()->GetValueAsObject(FName("Target"))))
			{
				// 공격한 대상 타겟으로 설정
				pAIController->GetBlackboardComponent()->SetValueAsObject(FName("Target"), EventInstigator->GetPawn());
			}
		}
	}
	
	// 공격한 대상(투사체 포함)의 반대방향으로 밀려남
	const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
	FVector LaunchVec = GetActorLocation() - PointDamageEvent->HitInfo.ImpactPoint;
	FVector LaunchForce = LaunchVec.GetSafeNormal() * 300.f;
	LaunchForce.Z = 0.f;
	LaunchCharacter(LaunchForce, false, false);

	TSoftObjectPtr<UAnimMontage> HitMontage = m_DataAssetInfo.LoadSynchronous()->GetMonAnimData(m_Type)->HitAnim_Nor;
	if (IsValid(HitMontage.LoadSynchronous()))
	{
		pAnimInst->Montage_Play(HitMontage.LoadSynchronous());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 피격애니메이션 로드 실패"));
	}

	USoundBase* HitSound = m_DataAssetInfo->GetMonSoundData(m_Type)->HitSound_Normal.LoadSynchronous();
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

void AMonster_Base::MonsterDead(AController* EventInstigator)
{
	AAIController* pAIController = Cast<AAIController>(GetController());
	pAIController->GetBrainComponent()->StopLogic(TEXT("Dead"));

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(EventInstigator->GetPawn());

	m_State = EMONSTER_STATE::DEAD;
	bIsDead = true;
	GetController()->UnPossess();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreAll"));
	GetMesh()->SetCollisionProfileName(TEXT("IgnoreAll"));
	// 록온 상태일 경우 해제
	if (pPlayer->GetbToggleLockOn())
	{
		pPlayer->BreakLockOn();
	}

	TArray<TObjectPtr<USceneComponent>> LockOnCompArr = GetMesh()->GetAttachChildren();
	if (LockOnCompArr.Num() <= 0)
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

	pPlayer->GainMonsterSoul(m_Info.Dropped_Soul);

	EITEM_ID eId;
	int32 iStack;
	// 랜덤으로 드롭아이템 지정
	float fRandNum = FMath::RandRange(1.f, 100.f);
	for (int32 i = 0; i < m_DropItemArr.Num(); ++i)
	{
		if (m_DropItemArr[i].ProbabilityBottom < fRandNum && fRandNum < m_DropItemArr[i].ProbabilityTop)
		{
			eId = m_DropItemArr[i].Item;
			iStack = m_DropItemArr[i].Stack;
			break;
		}
	}

	FActorSpawnParameters SpawnParams;
	// 스폰한 위치에 충돌이 발생할 경우 충돌이 발생하지 않는 가장 가까운 위치에 스폰
	// 스폰할 위치를 찾지 못하면 충돌 상관없이 원래 위치에 스폰
	TSubclassOf<AItem_Dropped_Base> Item = AItem_Dropped_Base::StaticClass();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator Rotator;
	FVector vDropLoc = GetActorLocation();
	vDropLoc.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 20.f;
	AItem_Dropped_Base* pDropItem = GetWorld()->SpawnActor<AItem_Dropped_Base>(Item, vDropLoc, Rotator, SpawnParams);
	pDropItem->SetDropItemID(eId);
	pDropItem->SetDropItemStack(iStack);

	USoundBase* DeadSound = m_DataAssetInfo->GetMonSoundData(m_Type)->DeadSound.LoadSynchronous();
	if ( IsValid(DeadSound) )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound, GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 사망사운드 로드 실패"));
	}

	// 5초 뒤 사망 이펙트 처리
	GetWorld()->GetTimerManager().SetTimer(DeadTimer, [this]()
	{
		bIsDead = true;

		fDeadEffectRatio += 0.05f;
		TArray<USceneComponent*> ChildMeshArr;
		GetMesh()->GetChildrenComponents(true, ChildMeshArr);
		if ( !ChildMeshArr.IsEmpty() )
		{
			for (USceneComponent* ChildMesh : ChildMeshArr)
			{
				USkeletalMeshComponent* ChildSkelMesh = Cast<USkeletalMeshComponent>(ChildMesh);
				ChildSkelMesh->SetScalarParameterValueOnMaterials(TEXT("EffectRatio"), fDeadEffectRatio);
			}
		}
		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("EffectRatio"), fDeadEffectRatio);
		if ( fDeadEffectRatio > 1.f )
		{
			GetWorld()->GetTimerManager().ClearTimer(DeadTimer);
			Destroy();
		}
	}, 
	0.1f, true, 5.f);
}

// 경직상태가 되는 몽타주들 재생 종료시
void AMonster_Base::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	TSoftObjectPtr<UAnimMontage> HitMontage = m_DataAssetInfo.LoadSynchronous()->GetMonAnimData(m_Type)->HitAnim_Nor;
	if (IsValid(HitMontage.LoadSynchronous()))
	{
		if (HitMontage == Montage)
		{
			// 피격 몽타주 재생 종료 후 1초 뒤 비헤이비어트리 재시작
			GetWorld()->GetTimerManager().ClearTimer(HitEndTimer);
			GetWorld()->GetTimerManager().SetTimer(HitEndTimer, [this]()
			{
				AAIController* pAIController = Cast<AAIController>(GetController());
				if ( IsValid(pAIController) )
				{
					if ( pAIController->GetBlackboardComponent() )
					{
						pAIController->GetBlackboardComponent()->SetValueAsBool(FName("bHitted"), false);
					}
				}
			},
			0.5f, false);

			return;
		}
	}
}

void AMonster_Base::OnBlockMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}

void AMonster_Base::MonsterAttackNormal()
{
	UAnimInstance_Monster_Base* pAnimInst = Cast<UAnimInstance_Monster_Base>(GetMesh()->GetAnimInstance());
	TSoftObjectPtr<UAnimMontage> AtkMontage = m_DataAssetInfo.LoadSynchronous()->GetMonAnimData(m_Type)->AtkAnim_Melee_Nor;
	if (IsValid(AtkMontage.LoadSynchronous()))
	{
		pAnimInst->Montage_Play(AtkMontage.LoadSynchronous());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 공격애니메이션 로드 실패"));
	}

	ChangeState(EMONSTER_STATE::IDLE);
}

void AMonster_Base::SetbLockedOn(bool _LockedOn)
{
	bLockedOn = _LockedOn;
	GetMesh()->SetRenderCustomDepth(_LockedOn);
	
	if (_LockedOn)
	{
		m_WidgetComponent->SetVisibility(_LockedOn);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(WidgetDisplayTimer);
		GetWorld()->GetTimerManager().SetTimer(WidgetDisplayTimer, [this]() {m_WidgetComponent->SetVisibility(true); }, 0.1f, false, 2.f);
	}
}

void AMonster_Base::MeleeAttackHitCheck()
{
	float AtkRadius = 20.f;
	if (m_Type == EMONSTER_TYPE::UndeadAssassin)
	{
		AtkRadius = 10.f;
	}
	else if (m_Type == EMONSTER_TYPE::Barghest)
	{
		AtkRadius = 50.f;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector vHitStart = GetMesh()->GetSocketLocation("Socket_HitStart");
	FVector vHitEnd = GetMesh()->GetSocketLocation("Socket_HitEnd");
	float fTraceHalfHeight = (vHitEnd - vHitStart).Size() * 0.5;

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

	if (bResult)
	{
		if ( HitResult.GetActor()->IsValidLowLevel() )
		{
			APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(HitResult.GetActor());

			if (!IsValid(pPlayer))
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

void AMonster_Base::ApplyPointDamage(FHitResult const& HitInfo, EATTACK_TYPE _AtkType)
{
	float iDamage;

	switch (_AtkType)
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
	if (pPlayer->GetbToggleGuard())
	{
		FVector vMonsterDir = GetActorForwardVector().GetSafeNormal();
		bool bBlocked = pPlayer->BlockEnemyAttack(iDamage, vMonsterDir);

		// 플레이어의 가드에 공격이 막힐 경우
		if (bBlocked)
		{
			UAnimInstance_Monster_Base* pAnimInst = Cast<UAnimInstance_Monster_Base>(GetMesh()->GetAnimInstance());
			TSoftObjectPtr<UAnimMontage> BlockMontage = m_DataAssetInfo.LoadSynchronous()->GetMonAnimData(m_Type)->BlockAnim;
			if (IsValid(BlockMontage.LoadSynchronous()))
			{
				pAnimInst->Montage_Play(BlockMontage.LoadSynchronous());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("몬스터 블록애니메이션 로드 실패"));
			}

			USoundBase* BlockSound = m_DataAssetInfo->GetMonSoundData(m_Type)->BlockSound.LoadSynchronous();
			if ( IsValid(BlockSound) )
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockSound, GetActorLocation());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("몬스터 블록사운드 로드 실패"));
			}

			return;
		}
	}

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);

	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.Normal, HitInfo, GetController(), this, DamageTypeBase);

	bAtkTrace = false;
}