// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_Monster_Base.h"
#include "../Monsters/Monster_Base.h"
#include "Kismet/GameplayStatics.h"

void UAnimInstance_Monster_Base::NativeInitializeAnimation()
{
	m_Monster = Cast<AMonster_Base>(TryGetPawnOwner());
}

void UAnimInstance_Monster_Base::NativeBeginPlay()
{
	FMonsterInfo MonInfo = m_Monster->GetMonsterInfo();
	if ( MonInfo.Type == EMONSTER_TYPE::Barghest )
	{
		m_HitMontage = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Monster/Animation/AM_Barghest_Hit.AM_Barghest_Hit'"));
	}
}

void UAnimInstance_Monster_Base::NativeUpdateAnimation(float _fDeltaTime)
{
	if (!IsValid(m_Monster))
	{
		return;
	}

	m_State = m_Monster->GetState();
	
}

void UAnimInstance_Monster_Base::AnimNotify_HitCheckStart()
{
	UE_LOG(LogTemp, Warning, TEXT("monster HitCheckStart Notify"));

	TSoftObjectPtr<UDA_MonsterSound> SoundDA = m_Monster->GetMonSoundDA().LoadSynchronous();
	TSoftObjectPtr<USoundBase> AtkSound = SoundDA->GetSoundMap().Find(m_Monster->GetMonsterInfo().Type)->AtkSound_Normal;
	if ( IsValid(AtkSound.LoadSynchronous()) )
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AtkSound.LoadSynchronous(), m_Monster->GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("몬스터 공격사운드 로드 실패"));
	}

	m_Monster->SetbAtkTrace(true);
}

void UAnimInstance_Monster_Base::AnimNotify_HitCheckEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("monster HitCheckEnd Notify"));
	m_Monster->SetbAtkTrace(false);
}

void UAnimInstance_Monster_Base::PlayHitAnimation()
{
	Montage_Play(m_HitMontage.LoadSynchronous());
}
