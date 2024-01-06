#pragma once

UENUM(BlueprintType)
enum class EInputActionType : uint8
{
	MOVE,
	ROTATION,
	JUMP,
	SPRINT,
	GUARD,
	DODGE,
	ATTACK,
	PRIMARYATTACK,
	PARRY,
	LOCKON,
	SWITCHLOCKON,
	OPENMENU,
};

UENUM(BlueprintType)
enum class ELockOnDirection : uint8
{
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class EMONSTER_TYPE : uint8
{
	Barghest,
	Centaur,
	Griffon,
	MountainDragon,
};

UENUM(BlueprintType)
enum class EMONSTER_STATE : uint8
{
	IDLE,
	LOOKAROUND,
	WALK,
	RUN,
	BITE,
	DIVE,
	HIT,
	DEAD,
};

UENUM(BlueprintType)
enum class EBOSS_STATE : uint8
{
	DEFAULT,
	FLY,
	WALK,
	RUN,
	BITE,
	CLAW,
	COMBO1,
	COMBO2,
	COMBO3,
	RANGEATTACK,
	FLYCLAW,
	DIVE,
	HIT,
	DEAD,
};

UENUM(BlueprintType)
enum class EITEM_TYPE : uint8
{
	ACCESSORIE,
	AMMO,
	ARMOR,
	CONSUMABLE,
	KEY,
	MISC,
	SPELL,
	THROWABLE,
	WEAPON,

	END,
};

UENUM(BlueprintType)
enum class EITEM_ID : uint8
{
	WEA_SWORD_KNIGHT,
	WEA_SHIELD_KNIGHT,

	ARM_CHEST_KNIGHT,
	ARM_GAUNTLET_KNIGHT,
	ARM_LEG_KNIGHT,

	CON_POTION_HP_LESS,
	CON_POTION_MP_LESS,
	CON_SOUL_LESS,

	AMM_ARROW_WOOD,

};