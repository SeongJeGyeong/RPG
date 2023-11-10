// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Struct.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Monsters_Base.generated.h"

UCLASS()
class RPGPORTFOLIO_API AMonsters_Base : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_Mesh;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FMonsterInfo	m_Info;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	EMONSTER_TYPE	m_Type;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	FDataTableRowHandle	m_TableRow;

	UPROPERTY()
	class UAnimInstance_Monster_Base* m_Anim;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	class ULockOnTargetComponent* m_TargetComp;

	UPROPERTY(Editanywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	class UFloatingPawnMovement* m_Movement;

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
	FMonsterInfo& GetMonsterInfo() { return m_Info; }


public:
	// Sets default values for this pawn's properties
	AMonsters_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& _Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
