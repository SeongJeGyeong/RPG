// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Boss_ChkDirection.h"
#include "AIController.h"
#include "../../Boss_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_Boss_ChkDirection::UBTS_Boss_ChkDirection()
{
	NodeName = TEXT("Check Target Direction");
}

void UBTS_Boss_ChkDirection::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnComp, _NodeMemory, _DT);

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
	vOffset = vOffset.GetSafeNormal();
	float fAngle = FVector::DotProduct(pBoss->GetActorForwardVector(), vOffset);
	// fAngle : 1에 가까울 수록 몬스터의 정면에 가까우며, -1에 가까울 수록 몬스터의 후면에 가까움
	// 0 = 몬스터의 90도 측면에 있음
	float fDegree = FMath::RadiansToDegrees(fAngle);

	_OwnComp.GetBlackboardComponent()->SetValueAsFloat(TEXT("TargetDirection"), fAngle);

	float Distance = FVector::Distance(pBoss->GetActorLocation(), pPlayer->GetActorLocation());

	float fAtkRange = _OwnComp.GetBlackboardComponent()->GetValueAsFloat(FName("AtkRange"));
	bool InAtkRange;
	if (Distance < fAtkRange)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("InAtkRange"), true);
		InAtkRange = true;
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("InAtkRange"), false);
		InAtkRange = false;
	}

	FColor AtkColor;
	InAtkRange ? AtkColor = FColor::Magenta : AtkColor = FColor::Cyan;
	DrawDebugSphere(GetWorld(), pBoss->GetActorLocation(), fAtkRange, 40, AtkColor, false, 0.4f);

	float fRangedAtkRange = _OwnComp.GetBlackboardComponent()->GetValueAsFloat(FName("RangedAtkRange"));
	bool InRangedAtkRange;
	if (Distance < fRangedAtkRange)
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("InRangedAtkRange"), true);
		InRangedAtkRange = true;
	}
	else
	{
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("InRangedAtkRange"), false);
		InRangedAtkRange = false;
	}

	FColor RangedAtkColor;
	InRangedAtkRange ? RangedAtkColor = FColor::Red : RangedAtkColor = FColor::Blue;
	DrawDebugSphere(GetWorld(), pBoss->GetActorLocation(), fRangedAtkRange, 40, RangedAtkColor, false, 0.4f);
}
