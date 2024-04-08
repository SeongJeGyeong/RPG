// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player_FootIK.generated.h"

struct FFootIKTraceInfo
{
	float fOffset;
	FVector vHitPoint;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPGPORTFOLIO_API UPlayer_FootIK : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class APlayer_Base_Knight* m_Player;

public:	
	// Sets default values for this component's properties
	UPlayer_FootIK();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FFootIKTraceInfo IKFootTrace(float _TraceDistance, FName _Socket);
	FRotator NormalToRotator(FVector _Vec);
};
