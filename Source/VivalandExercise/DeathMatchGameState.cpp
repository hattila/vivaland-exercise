// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMatchGameState.h"

#include "PointAndClickPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void ADeathMatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeathMatchGameState, PlayerScores);
}

void ADeathMatchGameState::InitializeNewPlayerScore(APlayerController* NewPlayer)
{
	FPlayerScore PlayerScore;
	PlayerScore.PlayerController = NewPlayer;
	PlayerScore.PlayerName = NewPlayer->PlayerState->GetPlayerName();
	PlayerScore.Score = 0;
	
	PlayerScores.Add(PlayerScore);
}

void ADeathMatchGameState::AddScoreToPlayer(APlayerController* PlayerController, int32 ScoreToAdd)
{
	// find the player in PlayerScores and add the score.
	for (int32 i = 0; i < PlayerScores.Num(); i++)
	{
		if (PlayerScores[i].PlayerController == PlayerController)
		{
			PlayerScores[i].Score += ScoreToAdd;
			break;
		}
	}

	// Sort the PlayerScores array by score
	PlayerScores.Sort([](const FPlayerScore& PlayerScoreA, const FPlayerScore& PlayerScoreB)
	{
		return PlayerScoreA.Score >= PlayerScoreB.Score;
	});
}

void ADeathMatchGameState::OnRep_PlayerScores()
{
	// iterate over every player and call the OnScoreChange function
	for (int32 i = 0; i < PlayerScores.Num(); i++)
	{
		APointAndClickPlayerController* PlayerController = Cast<APointAndClickPlayerController>(PlayerScores[i].PlayerController);
		if (PlayerController)
		{
			PlayerController->ServerRefreshScores();
		}
	}
}
