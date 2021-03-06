// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class AProjectile;

UENUM()
enum class EFiringState : uint8 { LOCKED, AIMING, RELOADING, OUT_OF_AMMO };

// Forwards Decleration
class UTankBarrel; 
class UTankTurret_;

// Hold parameters for barrel's properties and Elevate method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void Initialise(UTankBarrel* BarrelToSet, UTankTurret_* TurretToSet);

	UFUNCTION(BlueprintCallable)
	void AimAt(FVector HitLocation);

	UFUNCTION(BluePrintCallable)
		void Fire();

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	int32 GetRoundsLeft() const;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "State")
		EFiringState FiringState = EFiringState::RELOADING;

private:

	void MoveBarrel(FVector AimDirection);

	bool IsBarrelMoving();

	UPROPERTY(EditDefaultsOnly)
		float ReloadTime = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float LaunchSpeed = 4000; // TODO find sensible default, for now we use the value of 40 m/s

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		int32 RoundsLeft = 3;

	UTankBarrel* Barrel = nullptr;

	UTankTurret_* Turret = nullptr;

	FVector AimDirection;

	double LastFireTime = 0;
};
