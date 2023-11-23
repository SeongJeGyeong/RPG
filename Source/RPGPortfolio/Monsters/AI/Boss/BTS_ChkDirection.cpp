// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ChkDirection.h"
#include "AIController.h"
#include "../../Monster_Base.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTS_ChkDirection::UBTS_ChkDirection()
{
	NodeName = TEXT("Check Target Direction");
}

UBTS_ChkDirection::~UBTS_ChkDirection()
{
}

void UBTS_ChkDirection::TickNode(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DT)
{
	Super::TickNode(_OwnComp, _NodeMemory, _DT);

	AAIController* pController = _OwnComp.GetAIOwner();

	if (!IsValid(pController))
	{
		return;
	}

	AMonster_Base* pMonster = Cast<AMonster_Base>(pController->GetPawn());

	if (!IsValid(pMonster))
	{
		return;
	}

	ACharacter* pPlayer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	FVector vOffset = pPlayer->GetActorLocation() - pMonster->GetActorLocation();
	FVector Cross = FVector::CrossProduct(vOffset, pMonster->GetActorForwardVector());
	float fDir = FVector::DotProduct(Cross, pMonster->GetActorUpVector());
	vOffset = vOffset.GetSafeNormal();
	float fAngle = FVector::DotProduct(pMonster->GetActorForwardVector(), vOffset);

	UE_LOG(LogTemp, Warning, TEXT("player Direction : %f"), fDir);
	UE_LOG(LogTemp, Warning, TEXT("player Angle : %f"), fAngle);
	// fAngle : 1�� ����� ���� ������ ���鿡 ������, -1�� ����� ���� ������ �ĸ鿡 �����
	// 0 = ������ 90�� ���鿡 ����
	if (fAngle <= 0.f)
	{
		// ���� ���� ����
		if (fDir >= 1.f)
		{
			pController->GetBlackboardComponent()->SetValueAsInt(TEXT("TargetDirection"), 1);
		}
		// ���� ���� ������
		else if (fDir <= -1.f)
		{
			pController->GetBlackboardComponent()->SetValueAsInt(TEXT("TargetDirection"), -1);
		}
	}
	else
	{
		pController->GetBlackboardComponent()->SetValueAsInt(TEXT("TargetDirection"), 0);
	}
}
