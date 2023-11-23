// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Around.h"
#include "AIController.h"
#include "../../Monster_Griffon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_Griffon_Around::UBTT_Griffon_Around()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Griffon_Around, m_TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_Griffon_Around::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Griffon* pMonster = Cast<AMonster_Griffon>(pController->GetPawn());
	if (nullptr == pMonster)
	{
		return EBTNodeResult::Failed;
	}

	pMonster->GetCharacterMovement()->MaxWalkSpeed = 300.f;
	pMonster->ChangeBossState(EBOSS_STATE::DEFAULT);

	int32 iRand = FMath::RandRange(0, 1);
	if (iRand)
	{
		fAroundDir = -1.f;
	}
	else
	{
		fAroundDir = 1.f;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Griffon_Around::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (m_TargetKey.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Griffon* pMonster = Cast<AMonster_Griffon>(pController->GetPawn());
	if (nullptr == pMonster)
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
	}

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (nullptr == pPlayer)
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
	}

	FVector LookVector = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	pMonster->SetActorRotation(FMath::RInterpTo(pMonster->GetActorRotation(), TargetRot, _DeltaSeconds, 5.f));

	const FRotator YawRotation(0, TargetRot.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	pMonster->AddMovementInput(Direction, fAroundDir);
}
