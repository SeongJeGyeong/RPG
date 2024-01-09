#pragma once

#include "Enum.h"
#include "NiagaraSystem.h"
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
	uint8 Vigor;		// 생명력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Attunement;	// 집중력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Endurance;	// 지구력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Strength;	// 근력

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Dexterity;	// 기량

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Intelligence;// 지력
};

USTRUCT(Atomic, BlueprintType)
struct FCharacterBasePower
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	// Transient : 휘발성
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

USTRUCT(Atomic, BlueprintType)
struct FGameItemInfo
	: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EITEM_ID		ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EITEM_TYPE		Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString			ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString			Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString			IconImgPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			DEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Restore_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Restore_MP;

};

USTRUCT(Atomic, BlueprintType)
struct FGameItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EITEM_TYPE	Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> Niagara;

};