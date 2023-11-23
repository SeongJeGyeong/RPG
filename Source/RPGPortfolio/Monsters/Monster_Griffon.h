// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster_Base.h"
#include "Monster_Griffon.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API AMonster_Griffon : public AMonster_Base
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AMonster_Griffon();

private:
	TSoftObjectPtr<UAnimMontage> m_Montage;
	EBOSS_STATE		m_BState;

	bool bAtkTrace;
	bool bBossAttack;
	int32 iAtkPattern;

public:
	bool GetbAtkTrace() { return bAtkTrace; }
	void SetbAtkTrace(bool _AtkTrace) { bAtkTrace = _AtkTrace; }
	int32 GetiAtkPattern() { return iAtkPattern; }
	void SetiAtkPattern(int32 _AtkPattern) { iAtkPattern = _AtkPattern; }
	EBOSS_STATE	GetBossState() { return m_BState; }
	void ChangeBossState(EBOSS_STATE _BState) { m_BState = _BState; }

	void AttackHitCheck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
