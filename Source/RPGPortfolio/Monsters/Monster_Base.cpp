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

}

void AMonster_Base::OnConstruction(const FTransform& _Transform)
{
	Super::OnConstruction(_Transform);

	FMonsterInfo* pInfo = nullptr;

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

	if (m_WidgetComponent->IsWidgetVisible() && !bLockedOn)
	{
		fWidgetVisTime += DeltaTime * 1.f;
		if (fWidgetVisTime > 3.f)
		{
			m_WidgetComponent->SetVisibility(false);
			fWidgetVisTime = 0.f;
		}
	}
}

// Called to bind functionality to input
void AMonster_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMonster_Base::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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
		//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		//GetMesh()->SetSimulatePhysics(true);
	}

	return 0.0f;
}

void AMonster_Base::SetbLockedOn(bool _LockedOn)
{
	m_WidgetComponent->SetVisibility(_LockedOn);
	bLockedOn = _LockedOn;
}