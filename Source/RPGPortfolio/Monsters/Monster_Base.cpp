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

// Sets default values
AMonster_Base::AMonster_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AIControllerClass = AAIC_Monster_Base::StaticClass();
	m_AIController = Cast<AAIC_Monster_Base>(AAIC_Monster_Base::StaticClass());

	m_TargetComp = CreateDefaultSubobject<ULockOnTargetComponent>(TEXT("TargetComponent"));
	if (!IsValid(m_TargetComp))
	{
		UE_LOG(LogTemp, Error, TEXT("타겟 컴포넌트 생성 실패"));
	}
	m_TargetComp->SetupAttachment(GetMesh());

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

	// Lock On Mark
	m_LockOnMarker = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnMarker"));
	if ( !IsValid(m_LockOnMarker) )
	{
		UE_LOG(LogTemp, Error, TEXT("락온 마커 생성 실패"));
	}
	m_LockOnMarker->SetupAttachment(m_TargetComp);
	ConstructorHelpers::FClassFinder<UUserWidget> MarkerUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Monster/BPC_UI_LockOnMarker.BPC_UI_LockOnMarker_C'"));
	if ( MarkerUI.Succeeded() )
	{
		m_LockOnMarker->SetWidgetClass(MarkerUI.Class);
		//m_MarkerClass = MarkerUI.Class;
	}
	m_LockOnMarker->SetWidgetSpace(EWidgetSpace::Screen);
	m_LockOnMarker->SetDrawSize(FVector2D(50.f, 50.f));
}

void AMonster_Base::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);

	FMonsterInfo* pInfo;

	if (IsValid(m_TableRow.DataTable) && !m_TableRow.RowName.IsNone())
	{
		pInfo = m_TableRow.DataTable->FindRow<FMonsterInfo>(m_TableRow.RowName, TEXT(""));

		if (nullptr != pInfo)
		{
			m_Info = *pInfo;
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
	m_LockOnMarker->SetVisibility(false);
}

// Called every frame
void AMonster_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_State == EMONSTER_STATE::DEAD)
	{
		fDestroyRate += DeltaTime * 1.f;

		if (fDestroyRate > 3.f)
		{
			fDeadEffectRatio += DeltaTime * 0.3f;
			TArray<USceneComponent*> ChildMeshArr;
			GetMesh()->GetChildrenComponents(true, ChildMeshArr);
			if (!ChildMeshArr.IsEmpty())
			{
				for ( USceneComponent* ChildMesh : ChildMeshArr )
				{
					if ( "TargetComponent" == ChildMesh->GetName() )
					{
						continue;
					}
					USkeletalMeshComponent* ChildSkelMesh = Cast<USkeletalMeshComponent>(ChildMesh);
					ChildSkelMesh->SetScalarParameterValueOnMaterials(TEXT("EffectRatio"), fDeadEffectRatio);
				}
			}
			GetMesh()->SetScalarParameterValueOnMaterials(TEXT("EffectRatio"), fDeadEffectRatio);
			if (fDeadEffectRatio > 1.f)
			{
				Destroy();
			}
		}
	}

	if (bHitWait)
	{
		fHitWaitTime += DeltaTime * 1.f;

		if (fHitWaitTime > 1.f)
		{
			AAIController* pAIController = Cast<AAIController>(GetController());
			if ( IsValid(pAIController) )
			{
				if ( pAIController->GetBlackboardComponent() )
				{					
					//pAIController->GetBlackboardComponent()->SetValueAsBool(FName("WasHit"), false);
					pAIController->GetBrainComponent()->RestartLogic();
					bHitWait = false;
					fHitWaitTime = 0.f;
				}
			}
		}
	}

	if (m_WidgetComponent->IsWidgetVisible() && !bLockedOn)
	{
		fWidgetVisTime += DeltaTime * 1.f;
		if (fWidgetVisTime > 3.f)
		{
			m_WidgetComponent->SetVisibility(false);
			fWidgetVisTime = 0.f;
		}
	}

	if (bAtkTrace)
	{
		AttackHitCheck();
	}
}

// Called to bind functionality to input
void AMonster_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMonster_Base::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (m_State == EMONSTER_STATE::DEAD)
	{
		return 0.0f;
	}
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	FinalDamage = FMath::Clamp(FinalDamage - m_Info.PhysicDef, 0.f, FinalDamage);
	m_Info.CurHP = FMath::Clamp(m_Info.CurHP - FinalDamage, 0.f, m_Info.MaxHP);
	m_MonsterWidget->SetHPRatio(m_Info.CurHP / m_Info.MaxHP);
	m_WidgetComponent->SetVisibility(true);
	m_MonsterWidget->DisplayDMG(FinalDamage);
	fWidgetVisTime = 0.f;

	APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(DamageCauser);
	if (m_Info.CurHP <= 0.f && GetController())
	{
		m_State = EMONSTER_STATE::DEAD;
		GetController()->UnPossess();
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreAll"));
		m_TargetComp->DestroyComponent();
		m_LockOnMarker->DestroyComponent();

		pPlayer->GainMonsterSoul(m_Info.Dropped_Soul);
		bAtkTrace = false;

		TSoftObjectPtr<USoundBase> DeadSound = m_DataAssetInfo.LoadSynchronous()->GetSoundMap().Find(m_Type)->DeadSound;
		if (IsValid(DeadSound.LoadSynchronous()))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound.LoadSynchronous(), GetActorLocation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("몬스터 사망사운드 로드 실패"));
		}
		
		//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		//GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		// 블랙보드에 WasHit true로 전달
		AAIController* pAIController = Cast<AAIController>(GetController());
		if (IsValid(pAIController))
		{
			if (pAIController->GetBlackboardComponent())
			{
				//pAIController->GetBlackboardComponent()->SetValueAsBool(FName("WasHit"), true);
				m_State = EMONSTER_STATE::IDLE;
				pAIController->GetBrainComponent()->StopLogic("Hit");
			}
		}

		// 공격한 플레이어의 반대방향으로 밀려남
		FVector LaunchVec = GetActorLocation() - pPlayer->GetActorLocation();
		FVector LaunchForce = LaunchVec.GetSafeNormal() * 300.f;
		LaunchForce.Z = 0.f;

		UE_LOG(LogTemp, Warning, TEXT("MonsterLocation X: %f, Y: %f, Z: %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
		UE_LOG(LogTemp, Warning, TEXT("PlayerLocation X: %f, Y: %f, Z: %f"), pPlayer->GetActorLocation().X, pPlayer->GetActorLocation().Y, pPlayer->GetActorLocation().Z);
		UE_LOG(LogTemp, Warning, TEXT("LaunchVec X: %f, Y: %f, Z: %f"), LaunchForce.X, LaunchForce.Y, LaunchForce.Z);
		LaunchCharacter(LaunchForce, false, false);

		UAnimInstance_Monster_Base* pAnimInst = Cast<UAnimInstance_Monster_Base>(GetMesh()->GetAnimInstance());
		TSoftObjectPtr<UAnimMontage> HitMontage = m_DataAssetInfo.LoadSynchronous()->GetAnimMap().Find(m_Type)->HitAnim_Nor;
		if (IsValid(HitMontage.LoadSynchronous()))
		{
			pAnimInst->Montage_Play(HitMontage.LoadSynchronous());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("몬스터 피격애니메이션 로드 실패"));
		}

		TSoftObjectPtr<USoundBase> HitSound = m_DataAssetInfo.LoadSynchronous()->GetSoundMap().Find(m_Type)->HitSound_Normal;
		if (IsValid(HitSound.LoadSynchronous()))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound.LoadSynchronous(), GetActorLocation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("몬스터 피격사운드 로드 실패"));
		}

	}

	return 0.0f;
}

void AMonster_Base::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	TSoftObjectPtr<UAnimMontage> HitMontage = m_DataAssetInfo.LoadSynchronous()->GetAnimMap().Find(m_Type)->HitAnim_Nor;
	if (IsValid(HitMontage.LoadSynchronous()))
	{
		if (HitMontage == Montage)
		{
			UE_LOG(LogTemp, Warning, TEXT("피격 몽타주 종료"));

			bHitWait = true;
			fHitWaitTime = 0.f;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("피격이외의 몽타주 종료"));
		}
	}
}

void AMonster_Base::MonsterAttackNormal()
{
	UAnimInstance_Monster_Base* pAnimInst = Cast<UAnimInstance_Monster_Base>(GetMesh()->GetAnimInstance());
	TSoftObjectPtr<UAnimMontage> AtkMontage = m_DataAssetInfo.LoadSynchronous()->GetAnimMap().Find(m_Type)->AtkAnim_Melee_Nor;
	if (IsValid(AtkMontage.LoadSynchronous()))
	{
		pAnimInst->Montage_Play(AtkMontage.LoadSynchronous());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 공격애니메이션 로드 실패"));
	}

	TSoftObjectPtr<USoundBase> AtkSound = m_DataAssetInfo.LoadSynchronous()->GetSoundMap().Find(m_Type)->AtkSound_Normal;
	if (IsValid(AtkSound.LoadSynchronous()))
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AtkSound.LoadSynchronous(), GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 공격사운드 로드 실패"));
	}
	ChangeState(EMONSTER_STATE::IDLE);
}

void AMonster_Base::SetbLockedOn(bool _LockedOn)
{
	m_WidgetComponent->SetVisibility(_LockedOn);
	m_LockOnMarker->SetVisibility(_LockedOn);
	bLockedOn = _LockedOn;
}

void AMonster_Base::AttackHitCheck()
{
	float AtkRadius = 50.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector HitSphere = GetMesh()->GetSocketLocation("BARGHEST_Head");
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		HitSphere,
		HitSphere,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(AtkRadius),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), HitSphere, AtkRadius, 12, color);

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

			// 플레이어가 무적상태가 아닐 때만
			if (!pPlayer->GetbInvincible())
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
				UGameplayStatics::ApplyDamage(HitResult.GetActor(), m_Info.PhysicAtk, GetController(), this, UDamageType::StaticClass());

				TSoftObjectPtr<USoundBase> DmgSound = m_DataAssetInfo.LoadSynchronous()->GetSoundMap().Find(m_Type)->DmgSound_Normal;
				if ( IsValid(DmgSound.LoadSynchronous()) )
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), DmgSound.LoadSynchronous(), HitResult.GetActor()->GetActorLocation());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("몬스터 타격사운드 로드 실패"));
				}
				bAtkTrace = false;
			}
		}
	}

}
