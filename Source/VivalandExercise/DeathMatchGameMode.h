// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeathMatchGameState.h"
#include "GameFramework/GameModeBase.h"
#include "DeathMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VIVALANDEXERCISE_API ADeathMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void OnPlayerDies(APlayerController* ControllerOfDeadPlayer, APlayerController* KillerController);

protected:
	// get me the beginPlay ovveride
	virtual void BeginPlay() override;
	
	// get me the PostLogin override
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	ADeathMatchGameState* DeathMatchGameStateRef;

	void RespawnAtRandomPlayerStart(APlayerController* PlayerController);
};
