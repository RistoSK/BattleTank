// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


UTankTrack::UTankTrack() {

	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);

}


void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * otherComponent, FVector NormalImpulse, const FHitResult & Hit) {

	DriveTrack();
	ApplySidewaysForce();

	CurrentThrottle = 0;
}

void UTankTrack::ApplySidewaysForce()
{
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

	// Work-out the required acceleration  this frame to correct, we need minus cause we the correction is on the opposite direction of the slippage
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// Calculate and apply sideways for (F = m a)
	// TankBP      Tank(SceneComponent without the Cast)
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	// Diveded by two because we have 2 tracks in our tank
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;

	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle) {

	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);

}

void UTankTrack::DriveTrack() {

	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}
