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

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!IsValid(pPlayer))
	{
		return;
	}

	FVector vOffset = pPlayer->GetActorLocation() - pBoss->GetActorLocation();
	FVector Cross = FVector::CrossProduct(vOffset, pBoss->GetActorForwardVector());
	float fDir = FVector::DotProduct(Cross, pBoss->GetActorUpVector());

	UE_LOG(LogTemp, Warning, TEXT("fDir : %f"), fDir);


	if (pBoss->GetMonsterInfo().CurHP <= pBoss->GetMonsterInfo().MaxHP / 2)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 3);
		return;

		int iNum = FMath::RandRange(1, 10);
		if (iNum > 7)
		{
			_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 3);
			return;
		}
	}
	// 몬스터 기준 왼쪽
	if ( fDir >= 45.f )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 1);
	}
	// 몬스터 기준 오른쪽
	else if ( fDir <= -45.f )
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 2);
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 0);
	}

	UE_LOG(LogTemp, Warning, TEXT("OnBecomeRelevant"));
}
