// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Attack.h"
#include "AIController.h"
#include "../../Monster_Griffon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../MonsterAnim/Anim_Griffon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UBTT_Griffon_Attack::UBTT_Griffon_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Griffon_Attack::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	int32 PatternNum = _OwnComp.GetBlackboardComponent()->GetValueAsInt(FName("PatternKey"));

	AMonster_Griffon* pMonster = Cast<AMonster_Griffon>(_OwnComp.GetAIOwner()->GetPawn());
	if (nullptr == pMonster)
	{
		return EBTNodeResult::Succeeded;
	}
	m_Movement = pMonster->GetCharacterMovement();

	m_AnimInst = Cast<UAnim_Griffon>(pMonster->GetMesh()->GetAnimInstance());
	if (!IsValid(m_AnimInst))
	{
		return EBTNodeResult::Succeeded;
	}

	switch (PatternNum)
	{
	case 1:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO1);
		break;
	case 2:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO2);
		break;
	case 3:
		pMonster->ChangeBossState(EBOSS_STATE::COMBO3);
		break;
	default:
		break;
	}
	m_AnimInst->MeleeAttack(pMonster->GetBossState());

	return EBTNodeResult::InProgress;
}

void UBTT_Griffon_Attack::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (m_AnimInst->bIsAtkMove)
	{
		AMonster_Base* pMonster = Cast<AMonster_Base>(_OwnComp.GetAIOwner()->GetPawn());
		ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (!IsValid(pPlayer))
		{
			return;
		}

		FVector LookVector = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
		LookVector.Z = 0.f;
		FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
		pMonster->SetActorRotation(FMath::RInterpTo(pMonster->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 3.f));
		m_Movement->AddInputVector(pMonster->GetActorForwardVector() * 50.f * _DeltaSeconds);
	}

	if (!m_AnimInst->bBossAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimEnded"));
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
}
