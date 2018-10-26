// Copyright Risto Sardis Karhunen

#include "SprungWheel.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	MassWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Mass Wheel Constraint"));
	SetRootComponent(MassWheelConstraint);

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Axle->SetupAttachment(MassWheelConstraint);

	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	Wheel->SetupAttachment(Axle);

	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Axle Wheel Constraint"));
	AxleWheelConstraint->SetupAttachment(Axle);

	// Set physics parameters
	MassWheelConstraint->ComponentName1.ComponentName = FName("Mass");
	MassWheelConstraint->ComponentName2.ComponentName = FName("Wheel");

	MassWheelConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 100);
	MassWheelConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 100);
	MassWheelConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 100);

	MassWheelConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45);
	MassWheelConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45);
	MassWheelConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45);

	MassWheelConstraint->SetLinearPositionTarget(FVector(0, 0, 0));
	MassWheelConstraint->SetLinearPositionDrive(false, false, true);

	MassWheelConstraint->SetLinearVelocityTarget(FVector(0, 0, 0));
	MassWheelConstraint->SetLinearVelocityDrive(false, false, true);

	MassWheelConstraint->SetLinearDriveParams(500, 200, 0);

}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	// Allows us to get OnHit Events
	Wheel->SetNotifyRigidBodyCollision(true);
	Wheel->OnComponentHit.AddDynamic(this, &ASprungWheel::OnHit);

	SetupConstraints();

}

void ASprungWheel::SetupConstraints() {
	if (!GetAttachParentActor()) { return; }

	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());

	if (!BodyRoot) { return; }

	MassWheelConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);
	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->TickGroup == TG_PostPhysics) {
		TotalForceMagnitudeThisFrame = 0;
	}
	
}

void ASprungWheel::AddDrivingForce(float ForceMagnitude)
{
	TotalForceMagnitudeThisFrame += ForceMagnitude;
}

void ASprungWheel::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {

	ApplyForce();
}

void ASprungWheel::ApplyForce() {
	Wheel->AddForce(Axle->GetForwardVector() *TotalForceMagnitudeThisFrame);
}