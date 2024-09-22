// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_AtkMove.h"
#include "../Characters/Player_Base_Knight.h"
#include "Kismet/KismetMathLibrary.h"

UANS_AtkMove::UANS_AtkMove()
{
	bShouldFireInEditor = false;
}

void UANS_AtkMove::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (IsValid(m_Player))
	{
		if ( m_Player->GetbToggleLockOn() )
		{
			return;
		}

		if (m_Player->GetActorRotation() != rAtkRot)
		{
			fElapsedDuration += FrameDeltaTime;
			float fAlpha = exp2(FMath::Clamp(fElapsedDuration / fTotalDuration, 0.f, 1.f)) - 1;
			FRotator rLerpRot = FMath::Lerp(m_Player->GetActorRotation(), rAtkRot, fAlpha * fAlpha);
			m_Player->SetActorRotation(rLerpRot);
		}
	}
}

void UANS_AtkMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	fTotalDuration = TotalDuration - 0.1f;
	if (IsValid(MeshComp) && IsValid(MeshComp->GetOwner()))
	{
		m_Player = Cast<APlayer_Base_Knight>(MeshComp->GetOwner());
		if (!IsValid(m_Player))
		{
			UE_LOG(LogTemp, Error, TEXT("노티파이 스테이트 : 캐릭터 캐스팅 실패"));
		}
		else
		{
			if ( m_Player->GetbToggleLockOn() )
			{
				return;
			}

			m_Player->SetbInvalidInput(true);
			m_Player->SetbAtkRotate(false);
			if (!m_Player->GetvInputDir().IsZero())
			{
				FRotator InpRot = UKismetMathLibrary::MakeRotFromX(m_Player->GetvInputDir());
				rAtkRot = FRotator(0.f, m_Player->GetControlRotation().Yaw + InpRot.Yaw, 0.f);
			}
			else
			{
				rAtkRot = FRotator(0.f, m_Player->GetActorRotation().Yaw, 0.f);
			}
		}
	}
}

void UANS_AtkMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	rAtkRot = FRotator::ZeroRotator;
	fElapsedDuration = 0.f;
	if (!IsValid(m_Player))
	{
		UE_LOG(LogTemp, Error, TEXT("플레이어 로드 실패"));
		return;
	}
	m_Player->SetvInputDirZero();
}