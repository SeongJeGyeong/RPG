// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Griffon_Around.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_Griffon_Around::UBTT_Griffon_Around()
{
	bNotifyTick = true;

	m_TargetKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Griffon_Around, m_TargetKey), AActor::StaticClass());
	m_RecentTargetPos.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Griffon_Around, m_RecentTargetPos));
}

EBTNodeResult::Type UBTT_Griffon_Around::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTT_Griffon_Around::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	if (m_TargetKey.IsNone() || m_RecentTargetPos.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("추적 대상 설정 안됨"));
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	AAIController* pController = _OwnComp.GetAIOwner();

	ACharacter* pCharacter = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(m_TargetKey.SelectedKeyName));

	if (!IsValid(pCharacter))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());
	pMonster->GetCharacterMovement()->MaxWalkSpeed = 300.f;

	FVector TargetVect = pCharacter->GetOwner()->GetActorLocation() - (pMonster->GetOwner()->GetActorLocation() + FVector(0.f, 0.f, 100.f));
	FRotator TargetRot = TargetVect.GetSafeNormal().Rotation();
	//FRotator CurrentRot = GetControlRotation();
	// FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, LockonControlRotationRate);

	// Update control rotation to face target
	pMonster->GetController()->SetControlRotation(TargetRot);

	const FRotator Rotation = (pCharacter->GetOwner()->GetActorLocation() - pMonster->GetOwner()->GetActorLocation()).GetSafeNormal().Rotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	pMonster->AddMovementInput(Direction, 10.f);

}
