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
	ACharacter* pPlayer = Cast<ACharacter>(_OwnComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")));
	if ( !IsValid(pPlayer) )
	{
		return;
	}

	FVector vOffset = pPlayer->GetActorLocation() - pBoss->GetActorLocation();

	float fDot = FVector::DotProduct(pBoss->GetActorForwardVector(), vOffset);
	float fAngle = FMath::Acos(fDot);
	fAngle = FMath::RadiansToDegrees(fAngle);
	FVector vCross = FVector::CrossProduct(pBoss->GetActorForwardVector(), vOffset);
	if ( vCross.Z < 0.f )
	{
		fAngle *= -1;
	}

	int32 iDir = 4;
	if ( -90.f < fAngle && fAngle < 90.f )
	{
		iDir = 1;	// 정면
	}
	else if ( -135 < fAngle && fAngle <= -90.f )
	{
		iDir = 2;	// 왼쪽
	}
	else if ( 90.f <= fAngle && fAngle < 135.f )
	{
		iDir = 3;	// 오른쪽
	}

	_OwnComp.GetBlackboardComponent()->SetValueAsInt(TEXT("TargetDirection"), iDir);

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
	//DrawDebugSphere(GetWorld(), pBoss->GetActorLocation(), fRangedAtkRange, 40, RangedAtkColor, false, 0.4f);
}
