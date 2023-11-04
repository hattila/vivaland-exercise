// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerScore.h"
#include "GameFramework/GameStateBase.h"
#include "DeathMatchGameState.generated.h"

/**
 * 
 */
UCLASS()
class VIVALANDEXERCISE_API ADeathMatchGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void InitializeNewPlayerScore(APlayerController* NewPlayer);

	// create a getter for the player scores
	TArray<FPlayerScore> GetPlayerScores() const { return PlayerScores; }

	void AddScoreToPlayer(APlayerController* PlayerController, int32 ScoreToAdd);
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerScores)
	TArray<FPlayerScore> PlayerScores;
	
	UFUNCTION()
	void OnRep_PlayerScores();
};
