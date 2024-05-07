// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstance_Boss_Base.h"
#include "Anim_GreaterSpider.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnim_GreaterSpider : public UAnimInstance_Boss_Base
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = ( AllowPrivateAccess = "true" ))
	class ABoss_GreaterSpider* m_Boss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = ( AllowPrivateAccess = "true" ))
	class UCharacterMovementComponent* m_Movement;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

private:
	bool bBossAttack;
	bool bIsAtkMove;
	bool bIsTurn;
};
