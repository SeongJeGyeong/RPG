// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AtkTrace.h"
#include "../Characters/Player_Base_Knight.h"

void UANS_AtkTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if ( IsValid(m_Player) )
	{
		m_Player->AttackHitCheck();
	}
}

void UANS_AtkTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if ( IsValid(MeshComp) && IsValid(MeshComp->GetOwner()) )
	{
		m_Player = Cast<APlayer_Base_Knight>(MeshComp->GetOwner());
		if ( !IsValid(m_Player) )
		{
			UE_LOG(LogTemp, Error, TEXT("노티파이 스테이트 : 에디터 프리뷰"));
		}
	}
}

void UANS_AtkTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
}
