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
#include "Curves/CurveVector.h"
#include "../GameInstance_Base.h"
#include "../Manager/GISubsystem_SoundMgr.h"
#include "../Manager/Inventory_Mgr.h"
#include "../Manager/GISubsystem_EffectMgr.h"
#include "../Manager/GISubsystem_MonAssetMgr.h"

// Sets default values
AMonster_Base::AMonster_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	// false일 경우 TickFunction 자체가 레벨에 등록되지 않기 때문에 true로 처리
	PrimaryActorTick.bStartWithTickEnabled = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AAIC_Monster_Base::StaticClass();

	// widgetComponent
	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	if ( !IsValid(m_WidgetComponent) )
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetComponent Create Failed"));
	}
	m_WidgetComponent->SetupAttachment(GetRootComponent());
	m_WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	m_WidgetComponent->SetDrawSize(FVector2D(200.f, 200.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> MonsterUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Monster/BPC_UI_MonsterMain.BPC_UI_MonsterMain_C'"));
	if ( MonsterUI.Succeeded() )
	{
		m_WidgetComponent->SetWidgetClass(MonsterUI.Class);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDropTable(TEXT("/Script/Engine.DataTable'/Game/Blueprint/DataTable/DT_MonsterDropTable.DT_MonsterDropTable'"));
	if ( ItemDropTable.Succeeded() )
	{
		m_ItemTable = ItemDropTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveVector> TLCurve(TEXT("/Script/Engine.CurveVector'/Game/Blueprint/Timeline/Curve/HitTimelineCurve.HitTimelineCurve'"));
	if (TLCurve.Succeeded())
	{
		m_HitCurve = TLCurve.Object;
	}

	m_HitTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("HitTimelineComponent"));
	if ( m_HitTimeline ) {

		HitTimelineCallback.BindUFunction(this, TEXT("TimelineStep"));
		TimelineFinishCallback.BindUFunction(this, TEXT("TimelineFinished"));
		m_HitTimeline->AddInterpVector(m_HitCurve, HitTimelineCallback);
		m_HitTimeline->SetTimelineFinishedFunc(TimelineFinishCallback);
		m_HitTimeline->SetTimelineLength(0.3f);
		m_HitTimeline->SetLooping(false);
	}

	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	m_DetectRange = 800.f;
	m_AtkRange = 200.f;
}

// 에디터 상에서 호출됨(게임 실행 전)
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
		if (DropTableArr[i]->Monster == m_Type)
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
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AtkRange"), m_AtkRange);
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectRange"), m_DetectRange);
		}
		m_AnimInst = Cast<UAnimInstance_Monster_Base>(GetMesh()->GetAnimInstance());
		m_AnimInst->OnMontageEnded.AddDynamic(this, &AMonster_Base::OnHitMontageEnded);
	}

	if (!IsValid(m_WidgetComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 위젯 로드 실패"));
	}
	else
	{
		m_MonsterWidget = Cast<UUI_Monster>(m_WidgetComponent->GetWidget());
	}

	if (!IsValid(m_MonsterWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterWidget Invalid"));
	}
	else
	{
		m_MonsterWidget->SetName(m_Info.Name);
		m_MonsterWidget->SetHPRatio(1.f);
		m_Info.CurHP = m_Info.MaxHP;
	}
	m_WidgetComponent->SetVisibility(false);
	m_AnimAsset = GetGameInstance()->GetSubsystem<UGISubsystem_MonAssetMgr>()->GetMonAnimAsset(m_Type);

	// 랜덤으로 드롭아이템 지정
	float fRandNum = FMath::RandRange(1.f, 100.f);
	for ( int32 i = 0; i < m_DropItemArr.Num(); ++i )
	{
		if ( m_DropItemArr[i].ProbabilityBottom < fRandNum && fRandNum < m_DropItemArr[i].ProbabilityTop )
		{
			FGameItemInfo* pItemInfo = UInventory_Mgr::GetInst(GetWorld())->GetItemInfo(m_DropItemArr[i].Item);
			m_DropItemInfo.ID = m_DropItemArr[i].Item;
			m_DropItemInfo.Stack = m_DropItemArr[i].Stack;
			break;
		}
	}
	m_DropItemArr.Empty();
	RelLoc = GetMesh()->GetRelativeLocation();
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
	if (bMonDead)
	{
		return 0.0f;
	}
	bAtkTrace = false;

	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UDamageType_Base* pDamageType = Cast<UDamageType_Base>(DamageEvent.DamageTypeClass->GetDefaultObject());
	// 받은 공격타입에 따라 몬스터의 방어력 설정
	float fMonsterDef = 0.f;
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

	// 피격 사운드 재생
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GETMONSOUNDMAP(m_Type)->HitSound_Normal, GetActorLocation());

	// 사망 시
	if ( m_Info.CurHP <= 0.f && GetController() )
	{
		MonsterDead(EventInstigator);
		return 0.f;
	}

	// 비헤이비어 트리 피격 상태로 전환
	AAIController* pAIController = Cast<AAIController>(GetController());
	if ( IsValid(pAIController) )
	{
		if ( pAIController->GetBlackboardComponent() )
		{
			GetWorld()->GetTimerManager().ClearTimer(HitEndTimer);
			pAIController->GetBlackboardComponent()->SetValueAsBool(FName("bHitted"), true);
			if ( !IsValid(pAIController->GetBlackboardComponent()->GetValueAsObject(FName("Target"))) )
			{
				// 공격한 대상 타겟으로 설정
				pAIController->GetBlackboardComponent()->SetValueAsObject(FName("Target"), EventInstigator->GetPawn());
			}
		}
	}
	// 피격 시 몸체 진동
	m_HitTimeline->PlayFromStart();

	pAnimInst->Montage_Play(m_AnimAsset.HitAnim_Nor);

	// 피격 이펙트 스폰
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (!PointDamageEvent->HitInfo.BoneName.IsNone())
		{
			UGameplayStatics::SpawnEmitterAttached(GETHITEFFECT, GetMesh(), PointDamageEvent->HitInfo.BoneName, FVector::ZeroVector, FRotator::ZeroRotator, FVector(1.5f), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::AutoRelease);
		}
	}

	if (!bMonLockedOn)
	{
		GetWorld()->GetTimerManager().ClearTimer(WidgetDisplayTimer);
		// 락온되지 않은 상태에서 3초 동안 맞지 않았을 경우 위젯 사라지도록
		GetWorld()->GetTimerManager().SetTimer(WidgetDisplayTimer, [this]() {m_WidgetComponent->SetVisibility(false); }, 0.1f, false, 3.f);
	}
	
	// 공격한 대상(투사체 포함)의 반대방향으로 밀려남
	//const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
	//FVector LaunchVec = GetActorLocation() - PointDamageEvent->HitInfo.ImpactPoint;
	FVector LaunchVec = GetActorLocation() - DamageCauser->GetActorLocation();
	FVector LaunchForce = LaunchVec.GetSafeNormal() * 500.f;
	LaunchForce.Z = 0.f;
	LaunchCharacter(LaunchForce, false, false);

	return 0.0f;
}

void AMonster_Base::MonsterDead(AController* _EventInstigator)
{
	AAIController* pAIController = Cast<AAIController>(GetController());
	pAIController->GetBrainComponent()->StopLogic(TEXT("Dead"));

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(_EventInstigator->GetPawn());

	bMonDead = true;
	m_AnimInst->SetDeadAnim();
	m_WidgetComponent->DestroyComponent();
	GetController()->UnPossess();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreAll"));
	GetMesh()->SetCollisionProfileName(TEXT("IgnoreAll"));
	// 록온 상태일 경우 해제
	if (pPlayer->GetbIsLockOn())
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

	FActorSpawnParameters SpawnParams;
	// 스폰한 위치에 충돌이 발생할 경우 충돌이 발생하지 않는 가장 가까운 위치에 스폰
	// 스폰할 위치를 찾지 못하면 충돌 상관없이 원래 위치에 스폰
	TSubclassOf<AItem_Dropped_Base> Item = AItem_Dropped_Base::StaticClass();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FRotator Rotator;
	FVector vDropLoc = GetActorLocation();
	vDropLoc.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 20.f;

	AItem_Dropped_Base* pDropItem = GetWorld()->SpawnActor<AItem_Dropped_Base>(Item, vDropLoc, Rotator, SpawnParams);
	pDropItem->SetDropItemID(m_DropItemInfo.ID);
	pDropItem->SetDropItemStack(m_DropItemInfo.Stack);
	pDropItem->LoadImg();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GETMONSOUNDMAP(m_Type)->DeadSound, GetActorLocation());

	TArray<TObjectPtr<USceneComponent>> AttachCompArr = GetMesh()->GetAttachChildren();
	if ( AttachCompArr.IsEmpty() )
	{
		UE_LOG(LogTemp, Warning, TEXT("부착된 락온 컴포넌트 없음"));
	}
	else
	{
		for ( TObjectPtr<USceneComponent> AttachComp : AttachCompArr )
		{
			AttachComp->DestroyComponent();
		}
	}

	// 5초 뒤 사망 이펙트 처리
	GetWorld()->GetTimerManager().SetTimer(DeadTimer, [this]()
	{

		fDeadEffectRatio += 0.05f;
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
	if( Montage == m_AnimAsset.HitAnim_Nor )
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
						GetWorld()->GetTimerManager().ClearTimer(HitEndTimer);
					}
				}
		},
		0.8f, false);
	}

	return;
}

void AMonster_Base::OnBlockMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
}

void AMonster_Base::MonsterAttackNormal()
{
	GetMesh()->GetAnimInstance()->Montage_Play(m_AnimAsset.AtkAnim_Melee_Nor);
}

void AMonster_Base::PlayAtkBlockedAnim()
{
	GetMesh()->GetAnimInstance()->Montage_Play(m_AnimAsset.BlockAnim);
}

void AMonster_Base::SetMonLockedOn(bool _LockedOn)
{
	bMonLockedOn = _LockedOn;
	GetMesh()->SetRenderCustomDepth(_LockedOn);
	
	if (_LockedOn)
	{
		// 캐릭터가 타격 후 락온시 이전 타이머로 인해 위젯이 감춰질 수 있으므로
		GetWorld()->GetTimerManager().ClearTimer(WidgetDisplayTimer);
		m_WidgetComponent->SetVisibility(_LockedOn);
	}
	else
	{
		if (!bMonDead)
		{
			GetWorld()->GetTimerManager().ClearTimer(WidgetDisplayTimer);
			GetWorld()->GetTimerManager().SetTimer(WidgetDisplayTimer, [this]() 
				{
					m_WidgetComponent->SetVisibility(false);
					GetWorld()->GetTimerManager().ClearTimer(WidgetDisplayTimer);
				},
			0.1f, false, 2.f);
		}
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
	if ( bResult )
	{
		FVector const BlockingHitPoint = HitResult.Location;
		DrawDebugCapsule(GetWorld(), BlockingHitPoint, fTraceHalfHeight, AtkRadius, Rot, FColor::Red, false, 1.f);
	}

	//if (bDebug)
	//{
	// 	FVector CapsuleTip = Rot.RotateVector(FVector(0, 0, fTraceHalfHeight));
	//	DrawDebugCapsule(GetWorld(), PrevTraceLoc, fTraceHalfHeight, AtkRadius, Rot, FColor::Green, false, 1.f);
	//	DrawDebugCapsule(GetWorld(), vMidpoint, fTraceHalfHeight, AtkRadius, Rot, FColor::Green, false, 1.f);
	//	DrawDebugLine(GetWorld(), PrevTraceLoc + CapsuleTip, vMidpoint + CapsuleTip, FColor::Green, false, 1.f);
	//	DrawDebugLine(GetWorld(), PrevTraceLoc - CapsuleTip, vMidpoint - CapsuleTip, FColor::Green, false, 1.f);
	//}

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
			if (!pPlayer->CanBeDamaged())
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
	float iDamage = 0.f;

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

	// 공격 적중 시 잠시 경직
	m_AnimInst->Montage_Pause();
	GetWorld()->GetTimerManager().SetTimer(MonHitStiffTimer, [this]()
	{
		m_AnimInst->Montage_Resume(NULL);
		GetWorld()->GetTimerManager().ClearTimer(MonHitStiffTimer);
	},
	0.05f, false);

	TSubclassOf<UDamageType_Base> DamageTypeBase = UDamageType_Base::StaticClass();
	DamageTypeBase.GetDefaultObject()->SetAtkType(_AtkType);
	DamageTypeBase.GetDefaultObject()->SetAtkWeight(EATTACK_WEIGHT::LIGHT);
	UGameplayStatics::ApplyPointDamage(HitInfo.GetActor(), iDamage, HitInfo.ImpactPoint, HitInfo, GetController(), this, DamageTypeBase);

	bAtkTrace = false;
}

void AMonster_Base::TimelineStep(FVector _Value)
{
	GetMesh()->SetRelativeLocation(FVector(GetMesh()->GetRelativeLocation().X + (_Value.X * 10.f), GetMesh()->GetRelativeLocation().Y + (_Value.Y * 10.f), GetMesh()->GetRelativeLocation().Z));
}

void AMonster_Base::TimelineFinished()
{
	GetMesh()->SetRelativeLocation(RelLoc);
}