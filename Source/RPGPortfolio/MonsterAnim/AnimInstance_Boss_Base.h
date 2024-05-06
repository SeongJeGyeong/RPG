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
	UPROPERTY()
	class APlayer_Base_Knight* m_Player;
	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	float fMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	bool bIsMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	FVector	vLocalVelocity;	// 이동 블렌드 스페이스용 벡터

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	FVector	vPlayerLoc;		// 플레이어 바라보기 위해 사용

public:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info")
	bool bIsDead;

	float GetfMoveSpeed() const { return fMoveSpeed; }
	void SetfMoveSpeed(const float& _MoveSpeed) { fMoveSpeed = _MoveSpeed; }
	bool GetbIsMove() const { return bIsMove; }
	void SetbIsMove(const bool& _IsMove) { bIsMove = _IsMove; }
	FVector GetvLocalVelocity() const { return vLocalVelocity; }
	void SetvLocalVelocity(const FVector& _LocalVelocity) { vLocalVelocity = _LocalVelocity; }
	FVector GetvPlayerLoc() const { return vPlayerLoc; }
	void SetvPlayerLoc(const FVector& _PlayerLoc) { vPlayerLoc = _PlayerLoc; }


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDeltaTime) override;

	void SetDeadAnim() { bIsDead = true; }
};
