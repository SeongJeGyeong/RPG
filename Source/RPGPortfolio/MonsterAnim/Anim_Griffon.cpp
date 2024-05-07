// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_Griffon.h"
#include "../Monsters/Monster_Griffon.h"
#include "../Characters/Player_Base_Knight.h"
#include "Kismet/GameplayStatics.h"

void UAnim_Griffon::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnim_Griffon::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Monster = Cast<AMonster_Griffon>(TryGetPawnOwner());
	if (!IsValid(m_Monster))
	{
		UE_LOG(LogTemp, Error, TEXT("Griffon : m_Monster not found"));
	}
	else
	{
		m_Movement = m_Monster->GetCharacterMovement();
	}
}

void UAnim_Griffon::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);

	if (!IsValid(m_Movement) || !IsValid(m_Monster))
	{
		return;
	}

	fMoveSpeed = m_Movement->Velocity.Size2D();
	if (0.f < fMoveSpeed)
	{
		bIsMove = true;
	}
	else
	{
		bIsMove = false;
	}

	vLocalVelocity = m_Monster->GetRootComponent()->GetRelativeRotation().UnrotateVector(m_Movement->Velocity);
	vPlayerLoc = m_Player->GetActorLocation();

	if (m_Movement->IsFalling())
	{
		if (!Montage_IsPlaying(m_Montage.LoadSynchronous()))
		{
			bIsFly = true;
		}
	}
	else
	{
		if (!Montage_IsPlaying(m_Montage.LoadSynchronous()))
		{
			bIsFly = false;
		}
	}

	//m_State = m_Monster->GetBossState();
}

void UAnim_Griffon::MeleeAttack(EBOSS_STATE _BState)
{
	bBossAttack = true;

	switch (_BState)
	{
	case EBOSS_STATE::COMBO1:
		m_Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Claw_1Combo.AM_Claw_1Combo'"));
		break;
	case EBOSS_STATE::COMBO2:
		m_Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Claw_2Combo.AM_Claw_2Combo'"));
		break;
	case EBOSS_STATE::COMBO3:
		m_Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Claw_3Combo.AM_Claw_3Combo'"));
		break;
	default:
		break;
	}

	Montage_Play(m_Montage.LoadSynchronous());
}

void UAnim_Griffon::PlayTurnMontage(int32 _Dir)
{
	bIsTurn = true;

	if (_Dir == 1)
	{
		// Left
		m_Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Griffon_Trun_Left.AM_Griffon_Trun_Left'"));
	}
	else if (_Dir == -1)
	{
		// Right
		m_Montage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Griffon_Trun_Right.AM_Griffon_Trun_Right'"));
	}

	UE_LOG(LogTemp, Warning, TEXT("TurnStart"));
	Montage_Play(m_Montage.LoadSynchronous());
}

void UAnim_Griffon::AnimNotify_HitCheckStart()
{
	m_Monster->SetbAtkTrace(true);
}

void UAnim_Griffon::AnimNotify_HitCheckEnd()
{
	m_Monster->SetbAtkTrace(false);
}

void UAnim_Griffon::AnimNotify_Combo1()
{
	m_Monster->SetiAtkPattern(1);
}

void UAnim_Griffon::AnimNotify_Combo2()
{
	m_Monster->SetiAtkPattern(2);
}

void UAnim_Griffon::AnimNotify_Combo3()
{
	m_Monster->SetiAtkPattern(3);
}
