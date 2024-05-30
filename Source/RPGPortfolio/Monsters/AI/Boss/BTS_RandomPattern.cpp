// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RandomPattern.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../AIC_Monster_Base.h"

UBTS_RandomPattern::UBTS_RandomPattern()
{
	NodeName = TEXT("SelectRandomPattern");
	Interval = 1.f;

	bNotifyBecomeRelevant = true;
}

UBTS_RandomPattern::~UBTS_RandomPattern()
{
}

void UBTS_RandomPattern::OnBecomeRelevant(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::OnBecomeRelevant(_OwnComp, _NodeMemory);

	int iNum = FMath::RandRange(1, 10);

	if (iNum <= 3)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 1);
		UE_LOG(LogTemp, Warning, TEXT("Pattern 1"));
	}
	else if (iNum <= 8)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 2);
		UE_LOG(LogTemp, Warning, TEXT("Pattern 2"));
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("PatternNumber"), 3);
		UE_LOG(LogTemp, Warning, TEXT("Pattern 3"));
	}
}

void UBTS_RandomPattern::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnComp, _NodeMemory, _DT);
}
