// Fill out your copyright notice in the Description page of Project Settings.



#include "BTS_Detect.h"
#include "AIC_Monster_Base.h"
#include "../Monster_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_Detect::UBTS_Detect()
	: bDetect(false)
	, bIsAtkRange(false)
{
	// 감지된 플레이어가 탐지범위 내에 있는지 확인
	NodeName = TEXT("Detect Player");
	Interval = 0.5f;
}

UBTS_Detect::~UBTS_Detect()
{
}

void UBTS_Detect::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnComp, _NodeMemory, _DT);

	AAIC_Monster_Base* pController = Cast<AAIC_Monster_Base>(_OwnComp.GetAIOwner());

	if (!IsValid(pController))
	{
		return;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());

	if (!IsValid(pMonster))
	{
		return;
	}

	ACharacter* pPlayer = Cast<ACharacter>(pController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	float Distance = FVector::Distance(pMonster->GetActorLocation(), pPlayer->GetActorLocation());
	
	if (Distance < pController->GetLoseSightRadius())
	{
		bDetect = true;
	}
	else
	{
		pController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
		bDetect = false;
	}

	if (Distance < pMonster->GetMonsterInfo().AtkRange)
	{
		bIsAtkRange = true;
	}
	else
	{
		bIsAtkRange = false;
	}

	FColor color;
	bDetect ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().DetectRange, 40, color, false, 0.4f);

	FColor AtkColor;
	bIsAtkRange ? AtkColor = FColor::Magenta : AtkColor = FColor::Cyan;
	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().AtkRange, 40, AtkColor, false, 0.4f);

//#ifdef ENABLE_DRAW_DEBUG
//	FColor color;
//	bDetect ? color = FColor::Red : color = FColor::Green;
//	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().DetectRange, 40, color, false, 0.4f);
//
//	FColor AtkColor;
//	bIsAtkRange ? AtkColor = FColor::Magenta : AtkColor = FColor::Cyan;
//	DrawDebugSphere(GetWorld(), pMonster->GetActorLocation(), pMonster->GetMonsterInfo().AtkRange, 40, AtkColor, false, 0.4f);
//#endif
}
