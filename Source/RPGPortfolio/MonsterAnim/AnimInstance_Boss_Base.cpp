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

	OnMontageEnded.AddDynamic(this, &UAnimInstance_Boss_Base::OnAttackMontageEnded);
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
		bIsFly = true;
	}
	else
	{
		bIsFly = false;
	}

	m_State = m_Monster->GetBossState();
}

void UAnimInstance_Boss_Base::PlayAttackMontage(EBOSS_STATE _State)
{
	bIsAttack = true;

	switch (_State)
	{
	case EBOSS_STATE::COMBO1:
		m_AttackMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Claw_1Combo.AM_Claw_1Combo'"));
		break;
	case EBOSS_STATE::COMBO2:
		m_AttackMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Claw_2Combo.AM_Claw_2Combo'"));
		break;
	case EBOSS_STATE::COMBO3:
		m_AttackMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Claw_3Combo.AM_Claw_3Combo'"));
		break;
	default:
		break;
	}

	Montage_Play(m_AttackMontage.LoadSynchronous());
}

void UAnimInstance_Boss_Base::OnAttackMontageEnded(UAnimMontage* _Montage, bool _bInterrupted)
{
	bIsAttack = false;
}
