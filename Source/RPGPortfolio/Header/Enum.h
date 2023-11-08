#pragma once

UENUM(BlueprintType)
enum class EInputActionType : uint8
{
	MOVE,
	ROTATION,
	JUMP,
	SPRINT,
	ATTACK,
	GUARD,
	DODGE,
	LOCKON,
	CHANGELOCKON,
	PRIMARYATTACK,
};

UENUM(BlueprintType)
enum class ELockOnDirection : uint8
{
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right"),
};