// Fill out your copyright notice in the Description page of Project Settings.

#include "PointAndClickPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Navigation/PathFollowingComponent.h"

#include "DeathMatchGameMode.h"
#include "TankCharacter.h"
#include "Widget/ScoreboardWidget.h"

APointAndClickPlayerController::APointAndClickPlayerController()
{
	bShowMouseCursor = true;

	ConstructorHelpers::FClassFinder<UUserWidget> ScoreboardBPClass(TEXT("/Game/Widget/WBP_Scoreboard"));
	if (!ensure(ScoreboardBPClass.Class != nullptr)) return;

	ScoreboardClass = ScoreboardBPClass.Class;
}

void APointAndClickPlayerController::ServerPawnGotKilled_Implementation(APlayerController* KillerController)
{
	UnPossess();

	if (DeathMatchGameModeRef)
	{
		DeathMatchGameModeRef->OnPlayerDies(this, KillerController);	
	}
}

void APointAndClickPlayerController::ServerRefreshScores_Implementation()
{
	DoRefreshScores();
	ClientRefreshScores();
}

void APointAndClickPlayerController::ClientRefreshScores_Implementation()
{
	DoRefreshScores();
}

void APointAndClickPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		if (GameMode)
		{
			DeathMatchGameModeRef = Cast<ADeathMatchGameMode>(GameMode);
		}
	}

	if (!ensure(ScoreboardClass != nullptr)) return;
	
	ScoreboardWidget = CreateWidget<UScoreboardWidget>(GetWorld(), ScoreboardClass);
	ScoreboardWidget->AddToViewport();
}

void APointAndClickPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (EnhancedInputLocalPlayerSubsystem)
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ClickToMoveAction, ETriggerEvent::Triggered, this, &APointAndClickPlayerController::OnClickToMoveTriggered);
		EnhancedInputComponent->BindAction(ClickToMoveAction, ETriggerEvent::Completed, this, &APointAndClickPlayerController::OnClickToMoveCompleted);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APointAndClickPlayerController::OnShootTriggered);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &APointAndClickPlayerController::OnShootCompleted);
	}
}

void APointAndClickPlayerController::OnClickToMoveTriggered()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(
		ECollisionChannel::ECC_Visibility,
		false,
		HitResult
	);

	Destination = HitResult.ImpactPoint;

	// Easiest way to visualize where we are clicking
	DrawDebugSphere(
		GetWorld(),
		Destination,
		25.f,
		12,
		FColor::Red,
		false,
		-1.f,
		0.f,
		0.f
		);
}

void APointAndClickPlayerController::OnClickToMoveCompleted()
{
	/**
	 * Workaround to reinitialize PathFollowing on the client after a respawn.
	 * (Client path following is required for smooth movement)
	 * @see https://forums.unrealengine.com/t/simple-move-not-working-with-possessed-characters/437136/11
	 */
	UPathFollowingComponent* PathFollowingComp = FindComponentByClass<UPathFollowingComponent>();
	if (PathFollowingComp == nullptr)
	{
		PathFollowingComp = NewObject<UPathFollowingComponent>(this);
		PathFollowingComp->RegisterComponentWithWorld(GetWorld());
		PathFollowingComp->Initialize();
	}
	
	if (!PathFollowingComp->IsPathFollowingAllowed())
	{
		// After a client respawn we need to reinitialize the path following component
		// The default code path that sorts this out only fires on the server after a Possess
		PathFollowingComp->Initialize();
	}
	
	ServerMoveCharacterToDestination(Destination);
}

/**
 * Could be used for automatic fire.
 */
void APointAndClickPlayerController::OnShootTriggered()
{
	// UE_LOG(LogTemp, Warning, TEXT("ShootTriggered"));
}

void APointAndClickPlayerController::OnShootCompleted()
{
	APawn* ControlledPawn = GetPawn();

	// could be a shooting pawn interface, I'm casting now for simplicity
	ATankCharacter* TankCharacter = Cast<ATankCharacter>(ControlledPawn);
	if (TankCharacter)
	{
		TankCharacter->ServerShoot();
	}
}

void APointAndClickPlayerController::DoRefreshScores()
{
	ADeathMatchGameState* DeathMatchGameState = GetWorld()->GetGameState<ADeathMatchGameState>();
	if (!DeathMatchGameState) return;
	
	TArray<FPlayerScore> PlayerScores = DeathMatchGameState->GetPlayerScores();	
	ScoreboardWidget->RefreshScoreboard(PlayerScores);
}

/**
 * The Server performs the navigation and moves the character authoritatively.
 * Server calls the client to perform the same navigation.
 * The CharacterMovementComponent will handle the movement replication.
 *
 * @param DestinationToMoveTo 
 */
void APointAndClickPlayerController::ServerMoveCharacterToDestination_Implementation(FVector DestinationToMoveTo)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestinationToMoveTo);
	ClientMoveCharacterToDestination(DestinationToMoveTo);
}

/**
 * The Client needs to perform the same navigation in order achieve smooth movement.
 * Engine -> Navigation System -> Enable Client Navigation.
 * The Server will correct the client's movement if it is not valid.
 *
 * @param DestinationToMoveTo 
 */
void APointAndClickPlayerController::ClientMoveCharacterToDestination_Implementation(FVector DestinationToMoveTo)
{
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestinationToMoveTo);
}
