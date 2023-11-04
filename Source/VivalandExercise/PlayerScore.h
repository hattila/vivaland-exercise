#pragma once

#include "CoreMinimal.h"

#include "PlayerScore.generated.h"

USTRUCT()
struct FPlayerScore
{
	GENERATED_BODY()

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 Score;
};
