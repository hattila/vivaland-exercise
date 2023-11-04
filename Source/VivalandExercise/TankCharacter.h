// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TankCharacter.generated.h"

UCLASS()
class VIVALANDEXERCISE_API ATankCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATankCharacter();

	UFUNCTION(Server, Reliable)
	void ServerShoot();
	
	class USpringArmComponent* GetSpringArmComponent() const { return SpringArm; }
	class UCameraComponent* GetCameraComponent() const { return Camera; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	/**
	 * Could have Health, even a HealthComponent, but the requirements don't meet the complexity
	 */
	// UPROPERTY(EditDefaultsOnly, Category = "Combat")
	// float Health = 100.f;

	UFUNCTION(Server, Reliable)
	void ServerTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
