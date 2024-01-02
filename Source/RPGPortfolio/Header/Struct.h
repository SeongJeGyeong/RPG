#pragma once

#include "Enum.h"
#include "InputAction.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Struct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FInputActionData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInputActionType		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UInputAction>	Action;

};

USTRUCT(Atomic, BlueprintType)
struct FMonsterInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMONSTER_TYPE Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Def;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AtkRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BOSS_PerceiveRange;
};

USTRUCT(Atomic, BlueprintType)
struct FCharacterStatSheet : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Vigor;		// �����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Attunement;	// ���߷�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Endurance;	// ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Strength;	// �ٷ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Dexterity;	// �ⷮ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Intelligence;// ����
};

USTRUCT(Atomic, BlueprintType)
struct FCharacterBasePower
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	// Transient : �ֹ߼�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	float CurHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	float CurMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	float CurStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmountOfSoul;
};

