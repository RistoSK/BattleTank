// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankTurret_.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATank::AimAt(FVector HitLocation) {
	if (!ensure(TankAimingComponent)) { return; }

	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);

}

void ATank::Initialise(UTankBarrel* BarrelToSet, UTankTurret_ * TurretToSet) {

	if (!ensure(BarrelToSet && TurretToSet)) { return; }

	TankAimingComponent->Initialise(BarrelToSet, TurretToSet);
	Barrel = BarrelToSet;

}


void ATank::Fire() {

	if (!ensure(Barrel)) { return; }

	bool IsReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTime;

	if (ensure(IsReloaded)) { 

	auto BarrelRotator = Barrel->GetSocketRotation(FName("Projectile"));
	auto BarrelLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Spawn a projectile
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, BarrelLocation, BarrelRotator);

	Projectile->LaunchProjectile(LaunchSpeed);
	LastFireTime = FPlatformTime::Seconds();
	}
}