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

	m_LockOnMarker = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnMarker"));
	if ( !IsValid(m_WidgetComponent) )
	{
		UE_LOG(LogTemp, Error, TEXT("락온 마커 생성 실패"));
	}
	m_LockOnMarker->SetupAttachment(m_TargetComp);

	ConstructorHelpers::FClassFinder<UUserWidget> MarkerUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/UMG/Monster/BPC_UI_LockOnMarker.BPC_UI_LockOnMarker_C'"));
	if ( MarkerUI.Succeeded() )
	{
		m_LockOnMarker->SetWidgetClass(MarkerUI.Class);
		m_MarkerClass = MarkerUI.Class;
	}
	m_LockOnMarker->SetWidgetSpace(EWidgetSpace::Screen);
	m_LockOnMarker->SetDrawSize(FVector2D(50.f, 50.f));

	if (m_Type == EMONSTER_TYPE::Barghest)
	{
		ConstructorHelpers::FObjectFinder<UAnimSequence> HitAnim(TEXT("/Script/Engine.AnimSequence'/Game/QuadrapedCreatures/Barghest/Animations/BARGHEST_getHitNormal.BARGHEST_getHitNormal'"));
		if (HitAnim.Succeeded())
		{
			m_HitSequence = HitAnim.Object;
		}
		ConstructorHelpers::FObjectFinder<USoundBase> HitSound(TEXT("/Script/Engine.SoundCue'/Game/Blueprint/Monster/Sound/Barghest/SC_Barghest_Hit.SC_Barghest_Hit'"));
		if(HitSound.Succeeded())
		{
			m_HitSound = HitSound.Object;
		}
		
	}
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
		if (pAIController->GetBlackboardComponent())
		{
			pAIController->GetBlackboardComponent()->SetValueAsVector(FName("SpawnPosition"), GetActorLocation());
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AtkRange"), m_Info.AtkRange);
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectRange"), m_Info.DetectRange);
			pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("PerceiveRange"), m_Info.BOSS_PerceiveRange);
		}
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
			if (ChildMeshArr[0] != nullptr)
			{
				for (USceneComponent* ChildMesh : ChildMeshArr)
				{
					if ("TargetComponent" == ChildMesh->GetName())
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

	if (m_State == EMONSTER_STATE::HIT)
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
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

	if (m_Info.CurHP <= 0.f && GetController())
	{
		m_State = EMONSTER_STATE::DEAD;
		GetController()->UnPossess();
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("IgnoreAll"));
		m_TargetComp->DestroyComponent();
		m_LockOnMarker->DestroyComponent();
		APlayer_Base_Knight* pPlayer = Cast<APlayer_Base_Knight>(DamageCauser);
		pPlayer->GainMonsterSoul(m_Info.Dropped_Soul);
		USoundBase* DeadSound = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Blueprint/Monster/Sound/Barghest/kill2.kill2'"));
		if (IsValid(DeadSound))
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeadSound, GetActorLocation());
		}

		//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		//GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		UAnimInstance_Monster_Base* pAnimInst = Cast<UAnimInstance_Monster_Base>(GetMesh()->GetAnimInstance());
		pAnimInst->PlayHitAnimation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), m_HitSound, GetActorLocation());
	}

	return 0.0f;
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
			UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), m_Info.PhysicAtk, GetController(), this, UDamageType::StaticClass());
			bAtkTrace = false;
		}
	}
}
