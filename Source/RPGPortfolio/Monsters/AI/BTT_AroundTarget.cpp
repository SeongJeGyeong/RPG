// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AroundTarget.h"
#include "AIController.h"
#include "../../Monsters/Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_AroundTarget::UBTT_AroundTarget()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_AroundTarget, m_TargetKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTT_AroundTarget::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if ( nullptr == pMonster )
	{
		return EBTNodeResult::Failed;
	}

	pMonster->GetCharacterMovement()->MaxWalkSpeed = 300.f;
	pMonster->ChangeState(EMONSTER_STATE::WALK);

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
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	AAIController* pController = _OwnComp.GetAIOwner();
	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	if ( nullptr == pMonster )
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
	}

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if ( nullptr == pPlayer )
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
	}

	FVector LookVector = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	LookVector.Z = 0.f;

	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	FRotator CurrentRot = pMonster->GetControlRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, _DeltaSeconds, 20.f);
	pMonster->SetActorRotation(NewRot);
	
	/*FRotator newrot = ( pPlayer->GetActorLocation() - pMonster->GetActorLocation() ).Rotation();
	pMonster->SetActorRotation(newrot);*/

	const FRotator YawRotation(0, TargetRot.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	pMonster->AddMovementInput(Direction, fAroundDir);
}
