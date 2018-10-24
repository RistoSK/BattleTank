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

	if (RoundsLeft <= 0) {
		FiringState = EFiringState::OUT_OF_AMMO;
	}
	else if (IsReloading) {
		FiringState = EFiringState::RELOADING;
	}
	else if (IsBarrelMoving()) {
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

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
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
	// TODO having ensure will create error messages and small lag while running
	if (!(bHavingAimSolution)) { return; }

		AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();

		MoveBarrel(AimDirection);

	
	// No solution is found
}

void UTankAimingComponent::MoveBarrel(FVector TargetAimDirection) {

	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	// Work-out difference between current barrel rotation and TargetAimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = TargetAimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180) {
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else {
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}
bool UTankAimingComponent::IsBarrelMoving() {

	if (!ensure(Barrel)) { return false; }

	auto BarrelDirection = Barrel->GetForwardVector();

	return !BarrelDirection.Equals(AimDirection, 0.01f); // vectors are equal

}

void UTankAimingComponent::Fire() {


	if (FiringState == EFiringState::LOCKED || FiringState == EFiringState::AIMING) {

		if (!ensure(Barrel)) { return; }

		auto BarrelRotator = Barrel->GetSocketRotation(FName("Projectile"));
		auto BarrelLocation = Barrel->GetSocketLocation(FName("Projectile"));

		// Spawn a projectile
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, BarrelLocation, BarrelRotator);

		if (!ensure(Projectile))  { return; }
		Projectile->LaunchProjectile(LaunchSpeed);
		RoundsLeft--;
		LastFireTime = FPlatformTime::Seconds();
	}
}

