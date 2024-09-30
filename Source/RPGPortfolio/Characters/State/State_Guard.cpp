// Fill out your copyright notice in the Description page of Project Settings.


#include "State_Guard.h"
#include "../Player_Base_Knight.h"
#include "Kismet/GameplayStatics.h"

void State_Guard::Enter(APlayer_Base_Knight* Character)
{
	//Character->SetfGuardWeight(1.f);
	UGameplayStatics::PlaySoundAtLocation(Character, Character->GetSoundDA()->GetPlayerSound(EPlayerSound::GUARDBLOCK), Character->GetActorLocation());
	Character->GetMesh()->GetAnimInstance()->Montage_Play(Character->GetMontageDA()->GetPlayerMontage(EPlayerMontage::GUARD_LIGHT));

	//GetCharacterMovement()->AddImpulse(GetActorForwardVector() * -100.f, true);
}

void State_Guard::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->SetState(EPlayerStateType::IDLE);
	}
}

void State_Guard::Exit(APlayer_Base_Knight* Character)
{
	//Character->SetfGuardWeight(1.f);
}
