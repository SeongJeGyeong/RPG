// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API AMonster_Base : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FMonsterInfo	m_Info;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	EMONSTER_TYPE	m_Type;

	EMONSTER_STATE	m_State;

	EBOSS_STATE		m_BState;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle	m_TableRow;

	UPROPERTY()
	class UAnimInstance_Monster_Base* m_Anim;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	class ULockOnTargetComponent* m_TargetComp;

protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_BehaviroTree;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class UBlackboardData* m_Blackboard;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "AI")
	class AAIC_Monster_Base* m_AIController;

public:
	UBehaviorTree* GetBehaviorTree() { return m_BehaviroTree; }
	UBlackboardData* GetBlackboard() { return m_Blackboard; }
	const FMonsterInfo& GetMonsterInfo() { return m_Info; }
	EMONSTER_STATE GetState() { return m_State; }

	void ChangeState(EMONSTER_STATE _State) { m_State = _State; }
	void ChangeBossState(EBOSS_STATE _BState) { m_BState = _BState; }

public:
	// Sets default values for this character's properties
	AMonster_Base();

protected:

	virtual void OnConstruction(const FTransform& _Transform) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
