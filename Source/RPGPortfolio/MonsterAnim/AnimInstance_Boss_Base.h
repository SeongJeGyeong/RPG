// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Monsters/Boss_Base.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_Boss_Base.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UAnimInstance_Boss_Base : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	ABoss_Base* m_Boss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	UCharacterMovementComponent* m_Movement;

protected:
	UPROPERTY()
	class APlayer_Base_Knight* m_Player;
	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	float fMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	bool bIsMove;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = ( AllowPrivateAccess = "true" ))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	FVector	vLocalVelocity;	// 이동 블렌드 스페이스용 벡터

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	FVector	vPlayerLoc;		// 플레이어 바라보기 위해 사용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = ( AllowPrivateAccess = "true" ))
	int32	iRotateDir = 0;		// 회전 방향

	UPROPERTY()
	FQuat qOldOrientation;	// 현재 방향과 비교해서 캐릭터가 회전중인지 파악하기 위해 사용

public:
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
