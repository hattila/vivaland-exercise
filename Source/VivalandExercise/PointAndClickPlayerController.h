// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// #include "InputAction.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PointAndClickPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VIVALANDEXERCISE_API APointAndClickPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APointAndClickPlayerController();

	UFUNCTION(Server, Reliable)
	void ServerPawnGotKilled(APlayerController* KillerController);

	UFUNCTION(Server, Reliable)
	void ServerRefreshScores();
	
	UFUNCTION(Client, Reliable)
	void ClientRefreshScores();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* ClickToMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping")
	class UInputAction* ShootAction;

	virtual void SetupInputComponent() override;
	virtual void BeginPlay();

	void OnClickToMoveTriggered();
	void OnClickToMoveCompleted();
	void OnShootTriggered();
	void OnShootCompleted();

	UFUNCTION(Server, Reliable)
	void ServerMoveCharacterToDestination(FVector DestinationToMoveTo);

	UFUNCTION(Client, Reliable)
	void ClientMoveCharacterToDestination(FVector DestinationToMoveTo);

private:
	class ADeathMatchGameMode* DeathMatchGameModeRef;

	TSubclassOf<class UUserWidget> ScoreboardClass;

	class UScoreboardWidget* ScoreboardWidget;
	
	FVector Destination;

	void DoRefreshScores();

};
