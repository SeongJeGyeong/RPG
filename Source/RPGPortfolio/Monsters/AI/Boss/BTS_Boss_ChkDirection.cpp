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

	AAIController* pController = _OwnComp.GetAIOwner();
	if (!IsValid(pController))
	{
		return;
	}

	ABoss_Base* pBoss = Cast<ABoss_Base>(pController->GetPawn());
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

	UE_LOG(LogTemp, Warning, TEXT("player Direction : %f"), fDir);
	UE_LOG(LogTemp, Warning, TEXT("player Angle : %f"), fAngle);

	pController->GetBlackboardComponent()->SetValueAsFloat(TEXT("TargetDirection"), fAngle);

	float Distance = FVector::Distance(pBoss->GetActorLocation(), pPlayer->GetActorLocation());

	if (Distance < pBoss->GetMonsterInfo().AtkRange)
	{
		bInAtkRange = true;
	}
	else
	{
		bInAtkRange = false;
	}

	FColor AtkColor;
	bInAtkRange ? AtkColor = FColor::Magenta : AtkColor = FColor::Cyan;
	DrawDebugSphere(GetWorld(), pBoss->GetActorLocation(), pBoss->GetMonsterInfo().AtkRange, 40, AtkColor, false, 0.4f);
}
