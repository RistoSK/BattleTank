// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/Color.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/UnrealMathUtility.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

	
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	// Returns current health as a percentage of starting health, between 0 and 1
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "Health")
		FColor GetHealthBarColor() const;

	FTankDelegate OnDeath;

private:

	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	UPROPERTY(Editanywhere, Category = "Health")
		int32 StartingHealth = 100;


	UPROPERTY(VisibleAnywhere, Category = "Health")
		int32 CurrentHealth;

	UPROPERTY(Editanywhere, Category = "Health")
		FColor HealthBarColor = FColor::Cyan;

};
