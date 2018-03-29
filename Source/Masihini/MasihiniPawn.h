// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "MasihiniPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS(config=Game)
class AMasihiniPawn : public AWheeledVehicle
{
	GENERATED_BODY()

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	
public:
	AMasihiniPawn();

	/** Are we in reverse gear */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
	bool bInReverseGear;

	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End Pawn interface

	// Begin Actor interface
	virtual void Tick(float Delta) override;
protected:
	virtual void BeginPlay() override;

public:
	// End Actor interface

	/** Handle pressing forwards */
	void MoveForward(float Val);
	/** Handle pressing right */
	void MoveRight(float Val);
	
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();

	/** Handle Jump with vehicle */
	void OnJump();
	/** Handle Jump released*/
	void OnJumpRelease();

	/** Handle Rotation pressed */
	void OnRotationPressed();
	/** Handle Rotation released */
	void OnRotationReleased();

	/** Handle Animation Blueprint */
	void UpdateSkControls();
	
	static const FName LookUpBinding;
	static const FName LookRightBinding;

private:
	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;

	/** Are we in the air */
	bool bIsJumping;

	/** Are we rotating */
	bool bIsRotating;

	/** Jumping delay */
	FTimerHandle UnusedHandle;


public:
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};
