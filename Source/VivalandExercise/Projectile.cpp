// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 1.5f;
	bReplicates = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->MaxSpeed = 4000.f;
	ProjectileMovement->InitialSpeed = 4000.f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
) {

	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		// UE_LOG(LogTemp, Warning, TEXT("Projectile hit!, HitComp: %s, OtherActor: %s, OtherComp: %s, Got the instigator: %s"),
		// 	*HitComponent->GetName(),
		// 	*OtherActor->GetName(),
		// 	*OtherComp->GetName(),
		// 	GetInstigator() ? *GetInstigator()->GetName() : TEXT("No instigator")
		// );

		UGameplayStatics::ApplyDamage(
			OtherActor,
			1.f, // Damage
			GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
	}

	Destroy();
}