// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret_.h"
#include "TankBarrel.h"

void UTankTurret_::Rotate(float RelativeSpeed) {

	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);

	auto RotationChange = RelativeSpeed * RotationDegree * GetWorld()->DeltaTimeSeconds;
	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;

	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
}
