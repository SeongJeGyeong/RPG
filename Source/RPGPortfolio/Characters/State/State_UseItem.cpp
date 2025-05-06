// Fill out your copyright notice in the Description page of Project Settings.

#include "State_UseItem.h"
#include "../../Header/Struct.h"
#include "../Player_Base_Knight.h"
#include "../Comp/Player_InputComponent.h"
#include "../Comp/Player_InvenComponent.h"

void State_UseItem::Enter(APlayer_Base_Knight* Character)
{
	Character->Play_PlayerMontage(EPlayerMontage::USEITEM);
	// 아이템 사용후 대기시간 on
	Character->GetInvenComp()->ItemDelaytime(2.f);
}

void State_UseItem::Update(APlayer_Base_Knight* Character, float DeltaTime)
{
	if ( !Character->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() )
	{
		Character->GetInputComp()->SetState(EPlayerStateType::IDLE);
	}
}

void State_UseItem::Exit(APlayer_Base_Knight* Character)
{
}
