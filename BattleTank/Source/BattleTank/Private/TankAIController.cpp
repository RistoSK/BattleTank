// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"


void ATankAIController::BeginPlay() {

	Super::BeginPlay();

	auto PossesedTank = GetPossesedTank();
	auto PlayerTank = GetPlayerTank();

	if (!PossesedTank) {
		UE_LOG(LogTemp, Error, TEXT("AI Tank is NOT possesed"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("AI Tank %s is possesed and found the player %s"), *(PossesedTank->GetName()), *(PlayerTank->GetName()));

	}

}

ATank* ATankAIController::GetPossesedTank() const {

	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const {
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!PlayerPawn) {
		return nullptr;
	}
	return Cast<ATank>(PlayerPawn);
}