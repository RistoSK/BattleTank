// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {

	Super::BeginPlay();	

	for (TActorIterator<APawn> Itr(GetWorld(), APawn::StaticClass()); Itr; ++Itr) {
		APawn* tank = *Itr;
		if (tank != NULL) {
			AllTanks.Add(tank);
		}
	}
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
	//AllTanks.Remove(GetPawn());
	GetPawn()->DetachFromControllerPendingDestroy();
	UE_LOG(LogTemp, Warning, TEXT("RIP Enemy Tank"));
}

void ATankAIController::Tick(float DeltaTime)	{

	Super::Tick(DeltaTime);

	auto AITank = GetPawn();
	auto NextTankTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	for (APawn* Tank : AllTanks) {

		if(FVector::DistSquared(Tank->GetActorLocation(), AITank->GetActorLocation()) < FVector::DistSquared(NextTankTarget->GetActorLocation(), AITank->GetActorLocation())) {
			if (((Tank != AITank)
				&& (FVector::DistSquared(Tank->GetActorLocation(), AITank->GetActorLocation()) > AcceptanceRadius))
				&& (Cast<ATank>(Tank)->GetHealthPercent() > 0)) {
				NextTankTarget = Tank;
			}
		}
	}

	if (!(NextTankTarget && AITank)) { return; }
	
		MoveToActor(NextTankTarget, AcceptanceRadius);

		// Aim towards the closest tank
		auto AimingComponent = AITank->FindComponentByClass<UTankAimingComponent>();

		AimingComponent->AimAt(NextTankTarget->GetActorLocation());

		if (AimingComponent->GetFiringState() == EFiringState::LOCKED) {
			AimingComponent->Fire();
		}
		 
}

