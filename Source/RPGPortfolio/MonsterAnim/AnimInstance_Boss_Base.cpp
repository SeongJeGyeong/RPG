// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Boss_Base.h"
#include "../Monsters/Monster_Base.h"
#include "../Characters/Player_Base_Knight.h"
#include "Kismet/GameplayStatics.h"

void UAnimInstance_Boss_Base::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimInstance_Boss_Base::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Monster = Cast<AMonster_Base>(TryGetPawnOwner());
	if (!IsValid(m_Monster))
	{
		UE_LOG(LogTemp, Error, TEXT("보스 애님인스턴스 : 보스몬스터 찾지 못함"));
	}
	else
	{
		m_Movement = m_Monster->GetCharacterMovement();
	}

	m_Player = Cast<APlayer_Base_Knight>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(m_Player))
	{
		UE_LOG(LogTemp, Error, TEXT("보스 애님인스턴스 : 플레이어 찾지 못함"));
	}
}

void UAnimInstance_Boss_Base::NativeUpdateAnimation(float _fDeltaTime)
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
			//bIsFly = true;
		}
	}
	else
	{
		if (!Montage_IsPlaying(m_Montage.LoadSynchronous()))
		{
			//bIsFly = false;
		}
	}

	if (bAtkTrace)
	{
		CheckAttackTrace(iComboIdx);
	}

	m_State = m_Monster->GetBossState();
}

void UAnimInstance_Boss_Base::PlayAttackMontage(EBOSS_STATE _State)
{
	bBossAttack = true;

	switch (_State)
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

void UAnimInstance_Boss_Base::PlayTurnMontage(int32 _Dir)
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

void UAnimInstance_Boss_Base::CheckAttackTrace(int32 _ComboIdx)
{
	FVector vAtkCollisonLoc;

	switch (_ComboIdx)
	{
	case 1:
		vAtkCollisonLoc = m_Monster->GetMesh()->GetSocketLocation("GRIFFON_RightHand");
		break;
	case 2:
		vAtkCollisonLoc = m_Monster->GetMesh()->GetSocketLocation("GRIFFON_LeftHand");
		break;
	case 3:
		vAtkCollisonLoc = m_Monster->GetMesh()->GetSocketLocation("Head_Target");
		break;
	default:
		break;
	}

	float AtkRadius = 50.f;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, m_Monster);
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		vAtkCollisonLoc,
		vAtkCollisonLoc,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(AtkRadius),
		Params
	);

	FColor color;
	bResult ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), vAtkCollisonLoc, AtkRadius, 32, color);

	if (bResult)
	{
		if (HitResult.GetActor()->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit!!!"));
			bAtkTrace = false;
		}
	}

}