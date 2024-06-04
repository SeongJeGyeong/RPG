// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GS_AttackPattern.h"
#include "../../../Boss_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_GS_AttackPattern::UBTS_GS_AttackPattern()
{
	Interval = 1.f;
	bNotifyBecomeRelevant = true;
}

void UBTS_GS_AttackPattern::OnBecomeRelevant(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::OnBecomeRelevant(_OwnComp, _NodeMemory);

	ABoss_Base* pBoss = Cast<ABoss_Base>(_OwnComp.GetAIOwner()->GetPawn());
	if (!IsValid(pBoss))
	{
		return;
	}

	ACharacter* pPlayer = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if (!IsValid(pPlayer))
	{
		return;
	}

	FVector vOffset = pPlayer->GetActorLocation() - pBoss->GetActorLocation();
	FVector Cross = FVector::CrossProduct(vOffset, pBoss->GetActorForwardVector());
	float fDir = FVector::DotProduct(Cross, pBoss->GetActorUpVector());

	// 몬스터 기준 왼쪽
	if ( fDir >= 50.f )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("PatternNumber"), 1);
		UE_LOG(LogTemp, Warning, TEXT("Pattern1"));
	}
	// 몬스터 기준 오른쪽
	else if ( fDir <= -50.f )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("PatternNumber"), 2);
		UE_LOG(LogTemp, Warning, TEXT("Pattern2"));
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(FName("PatternNumber"), 0);
		UE_LOG(LogTemp, Warning, TEXT("Pattern0"));
	}

	UE_LOG(LogTemp, Warning, TEXT("OnBecomeRelevant"));
}
