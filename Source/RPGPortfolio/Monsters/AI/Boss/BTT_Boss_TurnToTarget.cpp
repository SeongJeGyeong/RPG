// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Boss_TurnToTarget.h"
#include "AIController.h"
#include "../../Boss_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_Boss_TurnToTarget::UBTT_Boss_TurnToTarget()
{
}

EBTNodeResult::Type UBTT_Boss_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	ABoss_Base* pBoss = Cast<ABoss_Base>(_OwnComp.GetAIOwner()->GetPawn());
	if ( nullptr == pBoss )
	{
		return EBTNodeResult::Failed;
	}
	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if ( nullptr == pPlayer )
	{
		return EBTNodeResult::Failed;
	}

	int32 iDir = _OwnComp.GetBlackboardComponent()->GetValueAsInt(TEXT("TargetDirection"));

	// 몬스터 기준 왼쪽
	if ( iDir == 2)
	{
		pBoss->SetiTurnDir(1);
	}
	// 몬스터 기준 오른쪽
	else if ( iDir == 3 )
	{
		pBoss->SetiTurnDir(2);
	}
	else
	{
		pBoss->SetiTurnDir(0);
	}

	FVector LookVector = pPlayer->GetActorLocation() - pBoss->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	pBoss->SetActorRotation(FMath::RInterpTo(pBoss->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.f));

	return EBTNodeResult::Succeeded;
}

void UBTT_Boss_TurnToTarget::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);
}
