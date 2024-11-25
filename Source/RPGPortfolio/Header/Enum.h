#pragma once

#include "Enum.generated.h"

class RPGPORTFOLIO_API Enum
{

};

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
	PARRY,
	LOCKON,
	SWITCHLOCKON,
	OPENMENU,
	ACTION,
	QUICKSLOTCHANGE,
	USELOWERQUICKSLOT,
	USESKILL_1,
	USESKILL_2,
};

UENUM(BlueprintType)
enum class EPlayerMontage : uint8
{
	DEFAULT,
	DODGE_FW,
	DODGE_BW,
	ATTACK,
	ATTACK_1,
	ATTACK_2,
	ATTACK_3,
	HEAVYATTACK,
	HEAVYATTACK_1,
	HEAVYATTACK_2,
	HEAVYATTACK_3,
	JUMPATTACK,
	PARRY,
	HIT,
	HIT_HEAVY,
	HIT_AIR,
	GUARDBREAK,
	USEITEM,
	SLASH_CUTTER,
	ACTION_ITEM,
	ACTION_PROP,
	GUARD_LIGHT,
	GUARD_HEAVY,
	DIE,
};

UENUM(BlueprintType)
enum class EPlayerSound : uint8
{
	EMPTY,
	PARRY,
	HIT,
	GUARDBREAK,
	GETITEM,
	USETHROW,
	USERESTORE,
	USESOUL,
	GUARDBLOCK,
	DIE,
};

UENUM(BlueprintType)
enum class EMenuSound : uint8
{
	EMPTY,
	MENU_OPEN,
	MENU_CLOSE,
	MENU_SELECT,
	MENU_CHANGE,
	ITEM_EQUIP,
	ITEM_UNEQUIP,
	SOUL_SUCK,
	SOUL_USE,
};

UENUM(BlueprintType)
enum class EATTACK_TYPE : uint8
{
	PHYSIC_MELEE,
	PHYSIC_RANGE,
	MAGIC_MELEE,
	MAGIC_RANGE,
};

UENUM(BlueprintType)
enum class ELockOnDirection : uint8
{
	Left,
	Right,
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
	GreaterSpider,
	ExiledWarrior
};

UENUM(BlueprintType)
enum class EMONSTER_STATE : uint8
{
	DEFAULT,
	IDLE,
	LOOKAROUND,
	WALK,
	RUN,
	BITE,
	ATTACK_NORMAL,
	ATTACK_HEAVY,
	DIVE,
	HIT,
	HITEND,
	DEAD,
};

UENUM(BlueprintType)
enum class EMON_MONTAGE : uint8
{
	EMPTY,
	ATTACK,
	BLOCKED,
	HIT,
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
	RUSH,
	HIT,
	DEAD,
	DEFEAT,
};

UENUM(BlueprintType)
enum class EGreaterSpider_STATE : uint8
{
	DEFAULT,
	WALK,
	RIGHTATTACK,
	LEFTATTACK,
	CENTERATTACK,
	RUSHATTACK,
	BODYSLAM,
	RANGEATTACK,
	HIT,
	DEAD,
	IDLE,
	PHASE2,
	SHOUT,
	DEADHIT,
};

UENUM(BlueprintType)
enum class EExiledWarrior_STATE : uint8
{
	DEFAULT,
	WALK,
	RUN,
	RIGHTATTACK,
	LEFTATTACK,
	CENTERATTACK,
	RUSHATTACK,
	RANGEATTACK,
	SUMMONMINION,
	STUNNED,
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
	EMPTY,

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
};

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	ITEM,
	DOOR,
	TREASUREBOX,
	LEVER,
	TELEPORT,
	INVESTIGATABLE,
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	NONE,
	BOMB,
	SLASH_CUTTER,
	SummonSpiderling,
};

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	NONE,
	RESTORE,
	GAIN,
	EXPLODE,
};

UENUM()
enum class ERecoveryType : uint8
{
	NONE,
	DEFAULT,
	SLOW,
};

UENUM(BlueprintType)
enum class ESkillName : uint8
{
	NONE,
	Slash_Cutter,
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	NONE,
	RESTORE,
	BUFF,
	DEBUFF,
	PHYSIC_MELEE,
	PHYSIC_RANGE,
	MAGIC_MELEE,
	MAGIC_RANGE,
};

UENUM(BlueprintType)
enum class EPlayerStateType : uint8
{
	NONE,
	IDLE,
	JUMP,
	SPRINT,
	DODGE,
	ATTACK,
	HEAVYATTACK,
	JUMPATTACK,
	HIT,
	USEITEM,
	ACTION,
	USESKILL_1,
	GUARD,
	GUARDBREAK,
	ATTACK_WAIT,
};

UENUM(BlueprintType)
enum class EATTACK_WEIGHT : uint8
{
	LIGHT,
	HEAVY,
};