// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathMatchGameMode.h"

#include "DeathMatchGameState.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

void ADeathMatchGameMode::OnPlayerDies(APlayerController* ControllerOfDeadPlayer, APlayerController* KillerController)
{
	if (!DeathMatchGameStateRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no DeathMatchGameStateRef!"));
		return;
	}
	
	DeathMatchGameStateRef->AddScoreToPlayer(KillerController, 1);

	// Kill indicator
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Green,
		FString::Printf(
			TEXT("%s killed %s"),
			*KillerController->PlayerState->GetPlayerName(),
			*ControllerOfDeadPlayer->PlayerState->GetPlayerName()
		)
	);

	RespawnAtRandomPlayerStart(ControllerOfDeadPlayer);
}

void ADeathMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	DeathMatchGameStateRef = GetGameState<ADeathMatchGameState>();

	if (DeathMatchGameStateRef)
	{
		// Initialize the PlayerScores array if it's empty
		if (DeathMatchGameStateRef->GetPlayerScores().Num() == 0)
		{
			for (int32 i = 0; i < GameState->PlayerArray.Num(); i++)
			{
				APlayerController* CurrentController = GameState->PlayerArray[i]->GetPlayerController();
				DeathMatchGameStateRef->InitializeNewPlayerScore(CurrentController);
			}
		}
	}
}

void ADeathMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (DeathMatchGameStateRef)
	{
		DeathMatchGameStateRef->InitializeNewPlayerScore(NewPlayer);
	}
	
}

void ADeathMatchGameMode::RespawnAtRandomPlayerStart(APlayerController* PlayerController)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	AActor* RandomPlayerStart = PlayerStarts[FMath::RandRange(0, PlayerStarts.Num() - 1)];

	APawn* NewPawn = GetWorld()->SpawnActor<APawn>(
		DefaultPawnClass,
		RandomPlayerStart->GetActorLocation(),
		RandomPlayerStart->GetActorRotation()
	);
	
	PlayerController->Possess(NewPawn);
}
