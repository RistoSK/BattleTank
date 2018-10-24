// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {

	Super::BeginPlay();	

}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!PossessedTank) { return; }

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	
	}
}

void ATankAIController::OnPossessedTankDeath() {
	
	if (!(GetPawn())) { return; }

	GetPawn()->DetachFromControllerPendingDestroy();
	UE_LOG(LogTemp, Warning, TEXT("RIP Enemy Tank"));
}

void ATankAIController::Tick(float DeltaTime)	{

	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto EnemyTank = GetPawn();
	

	if (!(PlayerTank && EnemyTank)) { return; }

		// TODO Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards the player
		auto AimingComponent = EnemyTank->FindComponentByClass<UTankAimingComponent>();

		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		if (AimingComponent->GetFiringState() == EFiringState::LOCKED) {
			AimingComponent->Fire();
		}
		 
}

