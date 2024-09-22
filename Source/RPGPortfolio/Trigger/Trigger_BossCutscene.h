// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Trigger_BossCutscene.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ATrigger_BossCutscene : public ATriggerBox
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( AllowPrivateAccess = "true" ))
	class ULevelSequence* m_LevelSeq;
	UPROPERTY()
	class ULevelSequencePlayer* m_SeqPlayer;

	UPROPERTY()
	AActor* m_TriggeredActor;
public:
	ATrigger_BossCutscene();

public:
	virtual void BeginPlay() override;

private:
	// OverlapDelegate
	UFUNCTION()
	void BeginOverlap(AActor* _TriggerActor, AActor* _OtherActor);

	UFUNCTION()
	void EndLevelSequence();
};
