// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Boss_Base.h"
#include "../Monsters/Monster_Base.h"
//#include "GameFramework/CharacterMovementComponent.h"
#include "../Characters/Player_Base_Knight.h"
#include "Kismet/GameplayStatics.h"

void UAnimInstance_Boss_Base::NativeInitializeAnimation()
{
	
}

void UAnimInstance_Boss_Base::NativeBeginPlay()
{
	m_Monster = Cast<AMonster_Base>(TryGetPawnOwner());

	if (IsValid(m_Monster))
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
	if (!IsValid(m_Movement) || !IsValid(m_Monster))
	{
		return;
	}

	fMoveSpeed = m_Movement->Velocity.Size2D();
	if (0.f < fMoveSpeed)
	{
		bIsMove = true;
		UE_LOG(LogTemp, Warning, TEXT("bIsMove : True"));
	}
	else
	{
		bIsMove = false;
		UE_LOG(LogTemp, Warning, TEXT("bIsMove : false"));
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
}