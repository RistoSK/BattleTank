// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay() {

	Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaTime)	{

	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto EnemyTank = Cast<ATank>(GetPawn());

	if (PlayerTank) {

		// TODO Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards the player
		EnemyTank->AimAt(PlayerTank->GetActorLocation());

		// fire if ready 
		// TODO don't fire every frame
		EnemyTank->Fire();
	}
	
}
