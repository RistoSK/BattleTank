// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankPlayerController.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class Atank;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
private:
		virtual void BeginPlay() override;

		virtual void Tick(float DeltaTime) override;

		ATank* GetPossesedTank() const;

		ATank* GetPlayerTank() const;
};
