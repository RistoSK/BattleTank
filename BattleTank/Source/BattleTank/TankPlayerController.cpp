// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay() {

	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("PlayerController Begin Play"));

}

void ATankPlayerController::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const {

	return Cast<ATank>(GetPawn());

}

void ATankPlayerController::AimTowardsCrosshair() {

	if (!GetControlledTank()) {
		return;
	}

	FVector HitLocation; // Out paramater
	if (GetSightRayHitLocation(HitLocation)) { //Has "side-effect", is going to line trace

		//UE_LOG(LogTemp, Warning, TEXT("Hitlocation: %s"), *HitLocation.ToString());

			// TODO Tell controlled tank to aim at this point
	}
	
}
// Get world location if linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{

	// Find the crosshair position
	int32  ViewportSizeX, ViewportSizeY;

	// Crosshair size
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);
	FVector LookDirection;

	// "De-project" the screen position of the crosshair to a world direction
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		UE_LOG(LogTemp, Warning, TEXT("Look direction %s"), *LookDirection.ToString());
	}


	

	// Line-trace along that look direction, and see what we hit (up to max range)

	OutHitLocation = FVector(1.f);
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const {

	FVector CameraWorldLocation; // TB Discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}
