// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Characters/Player_Base_Knight.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Knight.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstance_Knight : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	APlayer_Base_Knight* m_Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* m_Movement;
};
