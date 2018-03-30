// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MasihiniPawn.h"
#include "MasihiniWheelFront.h"
#include "MasihiniWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"

const FName AMasihiniPawn::LookUpBinding("LookUp");
const FName AMasihiniPawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

AMasihiniPawn::AMasihiniPawn()
{
	// Car Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	// Car Animation Blueprint
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UMasihiniWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UMasihiniWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UMasihiniWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UMasihiniWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	bInReverseGear = false;

	bIsJumping = false;
	bIsRotating = false;
}

void AMasihiniPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMasihiniPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMasihiniPawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AMasihiniPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AMasihiniPawn::OnHandbrakeReleased);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMasihiniPawn::OnJump);

	PlayerInputComponent->BindAction("Rotate", IE_Pressed, this, &AMasihiniPawn::OnRotationPressed);
	PlayerInputComponent->BindAction("Rotate", IE_Released, this, &AMasihiniPawn::OnRotationReleased);
}

void AMasihiniPawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
	//AddMovementInput(GetActorForwardVector(), Val);
}

void AMasihiniPawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
	//AddMovementInput(GetActorRightVector(), Val);
}

void AMasihiniPawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AMasihiniPawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AMasihiniPawn::OnJump()
{
	FVector *impulseForce = new FVector(0.0f,0.0f,1000.0f);
	FName boneName = FName(TEXT("None"));

	if (this->GetVelocity().Z >= 0.0f && !bIsJumping)
	{
		GetVehicleMovementComponent()->UpdatedPrimitive->SetPhysicsLinearVelocity(*impulseForce, true, boneName);
		bIsJumping = true;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMasihiniPawn::OnJumpRelease, 1.0f, false);
	}
}

void AMasihiniPawn::OnJumpRelease()
{
	bIsJumping = false;
}

void AMasihiniPawn::OnRotationPressed()
{
	bIsRotating = true;
	/*
	FBodyInstance *body = GetMesh()->GetBodyInstance();
	if (body)
	{
		body->bLockXTranslation = true;
		body->bLockYTranslation = true;
		body->bLockZTranslation = true;
		body->SetDOFLock(EDOFMode::SixDOF);
	}
	*/
}

void AMasihiniPawn::OnRotationReleased()
{
	bIsRotating = false;
	/*
	FBodyInstance *body = GetMesh()->GetBodyInstance();
	if (body)
	{
		body->bLockXTranslation = false;
		body->bLockYTranslation = false;
		body->bLockZTranslation = false;
		body->SetDOFLock(EDOFMode::Default);
	}
	*/
}

void AMasihiniPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

	if (bIsRotating)
	{
		FRotator relativeRotation = GetActorRotation();
		relativeRotation.Yaw += 1.0f;
		TeleportTo(GetActorLocation(), relativeRotation);
	}

}

void AMasihiniPawn::BeginPlay()
{
	Super::BeginPlay();
}

#undef LOCTEXT_NAMESPACE
