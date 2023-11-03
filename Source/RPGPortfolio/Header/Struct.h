#pragma once

#include "Enum.h"
#include "InputAction.h"
#include "CoreMinimal.h"
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