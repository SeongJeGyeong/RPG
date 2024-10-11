// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AroundTarget.h"
#include "AIController.h"
#include "../../Monsters/Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_AroundTarget::UBTT_AroundTarget()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_AroundTarget, m_TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_AroundTarget::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());
	if ( nullptr == pMonster )
	{
		return EBTNodeResult::Failed;
	}
	vCurTargetVec = pMonster->GetActorLocation();
	pMonster->GetCharacterMovement()->MaxWalkSpeed = 250.f;

	int32 iRand = FMath::RandRange(0, 1);
	if ( iRand )
	{
		fAroundDir = -1.f;
	}
	else
	{
		fAroundDir = 1.f;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_AroundTarget::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if ( m_TargetKey.IsNone() )
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());
	if ( nullptr == pMonster )
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	ACharacter* pPlayer = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(m_TargetKey.SelectedKeyName));
	if ( nullptr == pPlayer )
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}
	vCurTargetVec = pPlayer->GetActorLocation();

	FVector LookVector = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	LookVector.Z = 0.f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	FRotator NewRot = FMath::RInterpTo(pMonster->GetControlRotation(), TargetRot, _DeltaSeconds, 30.f);
	pMonster->SetActorRotation(NewRot);

	const FRotator YawRotation(0, TargetRot.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	pMonster->AddMovementInput(Direction, fAroundDir);

	_OwnComp.GetBlackboardComponent()->SetValueAsVector(FName("RecentTargetPos"), vCurTargetVec);
	_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("RecentPosSet"), true);
}
