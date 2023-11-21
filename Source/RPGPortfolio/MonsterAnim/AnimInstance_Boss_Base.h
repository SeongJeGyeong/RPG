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
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	float fMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool bIsMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	bool bIsFly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FVector	vLocalVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (AllowPrivateAccess = "true"))
	FVector	vPlayerLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	class AMonster_Base* m_Monster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	class UCharacterMovementComponent* m_Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	class APlayer_Base_Knight* m_Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UAnimMontage> m_Montage;

public:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info")
	EBOSS_STATE m_State;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info")
	bool bIsDead;

	void SetDeadAnim() { bIsDead = true; }

	bool bBossAttack;
	bool bIsTurn;
	bool bIsAtkMove;

	//FOnAttackEndDelegate OnAttackEnd;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

	void PlayAttackMontage(EBOSS_STATE _State);
	void PlayTurnMontage(int32 _Dir);

	UFUNCTION()
	void AnimNotify_BossAtkEnd();
	UFUNCTION()
	void AnimNotify_BossTurnEnd();
	UFUNCTION()
	void AnimNotify_MoveStart();
	UFUNCTION()
	void AnimNotify_MoveEnd();

};
