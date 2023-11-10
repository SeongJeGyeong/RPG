// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TurnToTarget_Monster.h"
#include "AIC_Monster_Base.h"
#include "../../Monsters/Monster_Base.h"
#include "../../Characters/Player_Base_Knight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_TurnToTarget_Monster::UBTT_TurnToTarget_Monster()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTT_TurnToTarget_Monster::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster_Base* pMonster = Cast<AMonster_Base>(OwnerComp.GetAIOwner()->GetPawn());

	if (nullptr == pMonster)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (nullptr == pPlayer)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	pMonster->SetActorRotation(FMath::RInterpTo(pMonster->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.f));

	return EBTNodeResult::Succeeded;
}
