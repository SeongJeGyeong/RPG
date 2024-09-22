// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../System/Interface/PlayerInteraction.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction_Lever.generated.h"

UCLASS()
class RPGPORTFOLIO_API AInteraction_Lever : public AActor, public IPlayerInteraction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	class UBoxComponent* m_Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = ( AllowPrivateAccess = "true" ))
	UStaticMeshComponent* m_Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence", meta = ( AllowPrivateAccess = "true" ))
	class ULevelSequence* m_LevelSeq;

	UPROPERTY()
	class ULevelSequencePlayer* m_SeqPlayer;	// 레벨시퀀스 플레이하는 객체

	UPROPERTY()
	AActor* m_TriggeredActor;

	FText tCommand_Key;
	FText tCommand_Name;
	bool bDoOnce;
	float fFadeRate = 1.f;

	FTimerHandle FadeTimer;

public:	
	// Sets default values for this actor's properties
	AInteraction_Lever();

protected:
	virtual void OnConstruction(const FTransform& _Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FText GetCommand_Key() const override { return tCommand_Key; }
	virtual FText GetCommand_Name() const override { return tCommand_Name; }

	virtual void Interaction(AActor* _InteractedActor = nullptr) override;

	UFUNCTION()
	void EndLevelSequence();
};
