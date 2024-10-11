#pragma once

#include "Enum.h"
#include "NiagaraSystem.h"
#include "InputAction.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Struct.generated.h"

class RPGPORTFOLIO_API Struct
{
};

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
	float PhysicAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Dropped_Soul;
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

USTRUCT(Atomic)
struct FEquipmentStat
{
	GENERATED_USTRUCT_BODY()

	float Helm_PhyDef = 0.f;
	float Helm_MagDef = 0.f;
	float Chest_PhyDef = 0.f;
	float Chest_MagDef = 0.f;
	float Gaunt_PhyDef = 0.f;
	float Gaunt_MagDef = 0.f;
	float Leg_PhyDef = 0.f;
	float Leg_MagDef = 0.f;
	float Wea_PhyAtk = 0.f;
	float Wea_MagAtk = 0.f;
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerAmountOfSoul
	: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

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

	// MultiLine = "true" : 데이터테이블 블루프린트에서 텍스트 입력 시 줄바꿈 가능하도록 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
	FString			Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString			IconImgPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			PhysicAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			MagicAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			PhysicDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			MagicDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Restore_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			Restore_MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32			Gained_Soul;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32			Maximum_Stack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32			Require_Str;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32			Require_Dex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32			Require_Int;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEffectType		EffectType;
};

USTRUCT()
struct FInvenItemRow
{
	GENERATED_USTRUCT_BODY()

	EITEM_ID	ID;
	uint32		Stack;
	EEQUIP_SLOT	EquipedSlot;
};

USTRUCT()
struct FInvenItemMap
{
	GENERATED_USTRUCT_BODY()

	TMap<EITEM_ID, FInvenItemRow> StorageMap;
};

USTRUCT()
struct FDroppedItemData
{
	GENERATED_USTRUCT_BODY()

	EITEM_ID	ID;
	TSoftObjectPtr<UTexture2D> ItemImg;
	uint32		Stack;
};

USTRUCT(Atomic, BlueprintType)
struct FMonsterSoundData
{
	GENERATED_USTRUCT_BODY()

	// 공격 타격 성공시 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> DmgSound_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> DmgSound_Alt;

	// 피격 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> HitSound_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> HitSound_Alt;

	// 사망 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> DeadSound;

};

USTRUCT(Atomic, BlueprintType)
struct FMonsterAnimData
{
	GENERATED_USTRUCT_BODY()

	// 근접공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> AtkAnim_Melee_Nor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> AtkAnim_Melee_Alt1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> AtkAnim_Melee_Alt2;

	// 원거리공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> AtkAnim_Range_Nor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> AtkAnim_Range_Alt1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> AtkAnim_Range_Alt2;

	// 피격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> HitAnim_Nor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> HitAnim_Alt;

	// 공격 방어에 막힐 경우 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> BlockAnim;
};

USTRUCT(Atomic, BlueprintType)
struct FMonsterItemDropTable
	: public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMONSTER_TYPE	Monster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EITEM_ID		Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32			Stack = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			ProbabilityBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float			ProbabilityTop;
};

USTRUCT(Atomic, BlueprintType)
struct FMonsterSoundAsset
{
	GENERATED_USTRUCT_BODY()

	// 공격 타격 성공시 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DmgSound_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DmgSound_Alt;

	// 피격 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* HitSound_Normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* HitSound_Alt;

	// 사망 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DeadSound;

};

USTRUCT(Atomic, BlueprintType)
struct FMonsterAnimAsset
{
	GENERATED_USTRUCT_BODY()

	// 근접공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AtkAnim_Melee_Nor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AtkAnim_Melee_Alt1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AtkAnim_Melee_Alt2;

	// 원거리공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AtkAnim_Range_Nor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AtkAnim_Range_Alt1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AtkAnim_Range_Alt2;

	// 피격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitAnim_Nor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HitAnim_Alt;

	// 공격 방어에 막힐 경우 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* BlockAnim;
};

USTRUCT(Atomic, BlueprintType)
struct FSkillAsset
{
	GENERATED_USTRUCT_BODY()

	// 스킬 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Animation;

	// 투사체 존재할 경우
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectile_Base> Projectile;

	// 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFXSystemAsset* Effect;

	// 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Sound;

	// 스킬 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillType SkillType;

	// 마나 소비량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MP_Consumption;

	// 스태미나 소비량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina_Consumption;

	// 기본 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
};