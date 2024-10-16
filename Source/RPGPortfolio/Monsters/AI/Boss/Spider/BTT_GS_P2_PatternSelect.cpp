// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GS_P2_PatternSelect.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../Boss_Base.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_GS_P2_PatternSelect::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	ABoss_Base* pBoss = Cast<ABoss_Base>(_OwnComp.GetAIOwner()->GetPawn());
	if ( !IsValid(pBoss) )
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* pPlayer = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if ( !IsValid(pPlayer) )
	{
		return EBTNodeResult::Failed;
	}

	float Distance = (pBoss->GetActorLocation() - pPlayer->GetActorLocation()).Size();
	int iNum = FMath::RandRange(1, 10);
	if ( iNum > 7 )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("Phase2Pattern"), 3);
	}
	else if ( iNum > 4 && Distance > 400.f )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("Phase2Pattern"), 2);
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("Phase2Pattern"), 1);
	}

	return EBTNodeResult::Succeeded;
}
