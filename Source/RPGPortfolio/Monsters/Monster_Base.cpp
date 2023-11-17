// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Base.h"
#include "../System/Component/LockOnTargetComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "AI/AIC_Monster_Base.h"

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
	m_TargetComp->SetupAttachment(GetRootComponent());
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
		pAIController->GetBlackboardComponent()->SetValueAsVector(FName("SpawnPosition"), GetActorLocation());
		pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AtkRange"), m_Info.AtkRange);
		pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectRange"), m_Info.DetectRange);
		pAIController->GetBlackboardComponent()->SetValueAsFloat(FName("PerceiveRange"), m_Info.BOSS_PerceiveRange);
	}
}

// Called every frame
void AMonster_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMonster_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

