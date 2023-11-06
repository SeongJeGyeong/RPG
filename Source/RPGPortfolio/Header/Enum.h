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
	ROLLING,
};