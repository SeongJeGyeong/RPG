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

	m_Player = Cast<APlayer_Base_Knight>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(m_Player))
	{
		UE_LOG(LogTemp, Error, TEXT("AnimInstance_Boss_Base : 플레이어 찾지 못함"));
	}
}

void UAnimInstance_Boss_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	Super::NativeUpdateAnimation(_fDeltaTime);
}