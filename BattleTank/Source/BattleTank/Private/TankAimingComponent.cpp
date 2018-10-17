// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankTurret_.h"

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// So that first fire is after initial reload
	LastFireTime = FPlatformTime::Seconds();

}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool IsReloading = (FPlatformTime::Seconds() - LastFireTime) < ReloadTime;

	if (IsReloading) {
		FiringState = EFiringState::RELOADING;
	}  else if(IsBarrelMoving()) {
		FiringState = EFiringState::AIMING;
	} else {
		FiringState = EFiringState::LOCKED;
	}
	
}

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret_ * TurretToSet) {

	if (!ensure(BarrelToSet && TurretToSet)) { return; }

	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation) {

	if (!ensure(Barrel)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Calculate the OutLaunchVelocity
	bool bHavingAimSolution = UGameplayStatics::SuggestProjectileVelocity(this,
																		  OutLaunchVelocity,
																		  StartLocation,
																		  HitLocation,
																		  LaunchSpeed,
																		  false,
																		  0,
																		  0,
																		  ESuggestProjVelocityTraceOption::DoNotTrace);

	if (ensure(bHavingAimSolution)) {

		AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();

		MoveBarrel(AimDirection);

	}
	// No solution is found
}

void UTankAimingComponent::MoveBarrel(FVector AimDirection) {

	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	// Work-out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);

}
bool UTankAimingComponent::IsBarrelMoving() {

	if (!ensure(Barrel)) { return false; }

	auto BarrelDirection = Barrel->GetForwardVector();

	return !BarrelDirection.Equals(AimDirection, 0.01f); // vectors are equal

}

void UTankAimingComponent::Fire() {


	if (FiringState != EFiringState::RELOADING) {

		if (!ensure(Barrel)) { return; }

		auto BarrelRotator = Barrel->GetSocketRotation(FName("Projectile"));
		auto BarrelLocation = Barrel->GetSocketLocation(FName("Projectile"));

		// Spawn a projectile
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, BarrelLocation, BarrelRotator);

		if (!ensure(Projectile)) { return; }
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}

