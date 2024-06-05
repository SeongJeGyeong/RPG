// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_GS_StartPhase2.h"
#include "../../../Boss_GreaterSpider.h"
#include "../../../../MonsterAnim/Anim_GreaterSpider.h"
#include "AIController.h"
#include "Camera/CameraShakeBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_GS_StartPhase2::UBTT_GS_StartPhase2()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_GS_StartPhase2::ExecuteTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory)
{
	Super::ExecuteTask(_OwnComp, _NodeMemory);

	ABoss_GreaterSpider* pBoss = Cast<ABoss_GreaterSpider>(_OwnComp.GetAIOwner()->GetPawn());
	if ( !IsValid(pBoss) )
	{
		return EBTNodeResult::Succeeded;
	}

	pBoss->PlayGSMontage(EGreaterSpider_STATE::PHASE2);
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	TSubclassOf<UCameraShakeBase> CameraShake = LoadClass<UCameraShakeBase>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Blueprint/Sequence/CameraShake/BPC_CamShake_GS_Phase2.BPC_CamShake_GS_Phase2_C'"));
	PlayerController->ClientPlayCameraShake(CameraShake);

	return EBTNodeResult::InProgress;
}

void UBTT_GS_StartPhase2::TickTask(UBehaviorTreeComponent& _OwnComp, uint8* _NodeMemory, float _DeltaSeconds)
{
	Super::TickTask(_OwnComp, _NodeMemory, _DeltaSeconds);

	ABoss_GreaterSpider* pBoss = Cast<ABoss_GreaterSpider>(_OwnComp.GetAIOwner()->GetPawn());
	if (!IsValid(pBoss))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	UAnim_GreaterSpider* AnimInst = Cast<UAnim_GreaterSpider>(pBoss->GetMesh()->GetAnimInstance());
	if (!IsValid(AnimInst))
	{
		FinishLatentTask(_OwnComp, EBTNodeResult::Failed);
		return;
	}

	if (!AnimInst->IsAnyMontagePlaying())
	{
		// idle 상태로
		pBoss->SetiTurnDir(0);
		_OwnComp.GetBlackboardComponent()->SetValueAsBool(FName("DoOnce"), false);
		FinishLatentTask(_OwnComp, EBTNodeResult::Succeeded);
		return;
	}
}
