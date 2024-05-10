// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Interaction.h"

UTrigger_Interaction::UTrigger_Interaction()
{
	SetCollisionProfileName(TEXT("InteractionTrigger"));
}

void UTrigger_Interaction::Interaction()
{
	UE_LOG(LogTemp, Warning, TEXT("레버 돌리기"));
}

FText UTrigger_Interaction::GetCommand_Key() const
{
	FText Command_Key;
	switch ( TriggerType )
	{
	case EInteractionType::ITEM:
		Command_Key = FText::FromString(TEXT("E"));
		break;
	case EInteractionType::DOOR:
		Command_Key = FText::FromString(TEXT("E"));
		break;
	case EInteractionType::TREASUREBOX:
		Command_Key = FText::FromString(TEXT("E"));
		break;
	case EInteractionType::LEVER:
		Command_Key = FText::FromString(TEXT("E"));
		break;
	case EInteractionType::TELEPORT:
		Command_Key = FText::FromString(TEXT("E"));
		break;
	case EInteractionType::INVESTIGATABLE:
		Command_Key = FText::FromString(TEXT("E"));
		break;
	default:
		break;
	}
	return Command_Key;
}

FText UTrigger_Interaction::GetCommand_Name() const
{
	FText Command_Name;
	switch ( TriggerType )
	{
	case EInteractionType::ITEM:
		Command_Name = FText::FromString(TEXT("획득한다"));
		break;
	case EInteractionType::DOOR:
		Command_Name = FText::FromString(TEXT("연다"));
		break;
	case EInteractionType::TREASUREBOX:
		Command_Name = FText::FromString(TEXT("연다"));
		break;
	case EInteractionType::LEVER:
		Command_Name = FText::FromString(TEXT("당긴다"));
		break;
	case EInteractionType::TELEPORT:
		Command_Name = FText::FromString(TEXT("이동한다"));
		break;
	case EInteractionType::INVESTIGATABLE:
		Command_Name = FText::FromString(TEXT("조사한다"));
		break;
	default:
		break;
	}

	return Command_Name;
}
