// Fill out your copyright notice in the Description page of Project Settings.

#include "TankCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "PointAndClickPlayerController.h"

ATankCharacter::ATankCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Camera should not rotate with the character
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxAcceleration = 1000.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	/**
	 * This is needed in order to accel-decel to work. It is protected, so I've just ticked it on in BP_Tank for now.
	 */
	// GetCharacterMovement()->bUseAccelerationForPaths = true; // 
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 2000.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;
	 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ATankCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	OnTakeAnyDamage.AddDynamic(this, &ATankCharacter::ServerTakeDamage);
}

void ATankCharacter::ServerShoot_Implementation()
{
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation()
	);
	Projectile->SetOwner(this);
	Projectile->SetInstigator(this);
}

void ATankCharacter::ServerTakeDamage_Implementation(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser
) {
	// could check damage against health, but we are one hit wonders here

	APointAndClickPlayerController* PointAndClickPlayerController = Cast<APointAndClickPlayerController>(GetController());
	APlayerController* KillerController = Cast<APlayerController>(InstigatedBy);
	
	if (PointAndClickPlayerController && KillerController)
	{
		PointAndClickPlayerController->ServerPawnGotKilled(KillerController);
	}
	
	Destroy();
}
