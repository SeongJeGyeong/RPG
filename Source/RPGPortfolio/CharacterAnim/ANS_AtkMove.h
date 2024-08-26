// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AtkMove.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UANS_AtkMove : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UANS_AtkMove();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref", meta = ( AllowPrivateAccess = "true" ))
	class APlayer_Base_Knight* m_Player;

	FRotator rAtkRot;
	float fTotalDuration;
	float fElapsedDuration;

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
