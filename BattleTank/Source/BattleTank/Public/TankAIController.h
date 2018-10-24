// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPlayerController.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
		UFUNCTION()
		void OnPossessedTankDeath();

		virtual void BeginPlay() override;

		virtual void Tick(float DeltaTime) override;

		virtual void SetPawn(APawn* InPawn) override;

		// How close can the AI tank get
		UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 9000;
	
};
