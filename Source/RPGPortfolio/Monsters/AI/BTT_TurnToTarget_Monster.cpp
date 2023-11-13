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
	Super::ExecuteTask(OwnerComp, NodeMemory);
	//EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}

void UBTT_TurnToTarget_Monster::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());

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
}
