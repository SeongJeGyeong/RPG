// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequencePlayer.h"
#include "LSP_Cutscene.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API ULSP_Cutscene : public ULevelSequencePlayer
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	AActor* ScenePlayActor;
public:
	AActor* GetPlayActor() { return ScenePlayActor; }
	void SetPlayActor(AActor* _Actor) { ScenePlayActor = _Actor; }
};
