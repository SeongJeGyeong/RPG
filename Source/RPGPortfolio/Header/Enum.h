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
	HEAVYTOGGLE,
	HEAVYATTACK,
	PARRY,
	LOCKON,
	SWITCHLOCKON,
	OPENMENU,
	ACTION,
	BACKTOPREV,
	QUICKSLOTCHANGE,
	USELOWERQUICKSLOT,
};

UENUM(BlueprintType)
enum class EPlayerMontage : uint8
{
	DODGE_FW,
	DODGE_BW,
	ATTACK,
	HEAVYATTACK,
	JUMPATTACK,
	PARRY,
	HIT,
	GUARDBREAK,
	USEITEM,
};

UENUM(BlueprintType)
enum class EPlayerSound : uint8
{
	PARRY,
	HIT,
	GUARDBREAK,
	GETITEM,
	USEITEM,
};

UENUM(BlueprintType)
enum class EMenuSound : uint8
{
	MENU_OPEN,
	MENU_CLOSE,
	MENU_SELECT,
	MENU_CHANGE,
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
	UndeadAssassin,
	HeavyKnight,
	TaurusDemon,
};

UENUM(BlueprintType)
enum class EMONSTER_STATE : uint8
{
	IDLE,
	LOOKAROUND,
	WALK,
	RUN,
	BITE,
	ATTACK_NORMAL,
	ATTACK_HEAVY,
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
	ALL,

	CONSUMABLE,
	WEAPON,
	SHIELD,
	ARM_HELM,
	ARM_CHEST,
	ARM_GAUNTLET,
	ARM_LEGGINGS,
	ACCESSORIE,
	ARROWS,
	SPELL,
	KEY,
	MISC,

	END,
};

UENUM(BlueprintType)
enum class EITEM_ID : uint8
{
	ARM_HELM_KNIGHT,
	ARM_CHEST_KNIGHT,
	ARM_GAUNTLET_KNIGHT,
	ARM_LEG_KNIGHT,

	ARR_ARROW_WOOD,

	CON_POTION_HP_LESS,
	CON_POTION_MP_LESS,
	CON_SOUL_LESS,

	WEA_SHIELD_KNIGHT,
	WEA_SWORD_KNIGHT,

	END,
};

UENUM(BlueprintType)
enum class EEQUIP_SLOT : uint8
{
	WEAPON_1,
	WEAPON_2,
	WEAPON_3,

	SHIELD_1,
	SHIELD_2,
	SHIELD_3,

	ARROW,
	BOLT,

	HELM,
	CHEST,
	GAUNTLET,
	LEGGINGS,

	ACCESSORIE_1,
	ACCESSORIE_2,
	ACCESSORIE_3,
	ACCESSORIE_4,

	CONSUMABLE_1,
	CONSUMABLE_2,
	CONSUMABLE_3,
	CONSUMABLE_4,
	CONSUMABLE_5,

	EMPTY,
};