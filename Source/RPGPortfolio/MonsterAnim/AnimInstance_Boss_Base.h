// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Boss_Base.generated.h"

//DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstance_Boss_Base : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	class APlayer_Base_Knight* m_Player;
	TSoftObjectPtr<UAnimMontage> m_Montage;
public:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info")
	bool bIsDead;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

	void SetDeadAnim() { bIsDead = true; }
};
