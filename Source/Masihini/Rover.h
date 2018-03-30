// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CoreMinimal.h"
#include "Rover.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAudioComponent;

UCLASS()
class MASIHINI_API ARover : public APawn
{
	GENERATED_BODY()

private:

#pragma region Components
	/** Skeletal Component that will have the mesh and Root Component */
	UPROPERTY(EditDefaultsOnly)
		USkeletalMeshComponent* Mesh;

	/** Skeletal Component that will have the weapon mesh */
	UPROPERTY(EditDefaultsOnly)
		USkeletalMeshComponent* Weapon;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* RoverGlass;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
		UAudioComponent* EngineSound;

	

#pragma endregion

#pragma region Variables

	/** Are we in the air */
	bool bIsJumping;

	/** Are we rotating */
	bool bIsRotating;

	/** Are we stopped */
	bool bIsStopped;

	/** Time Handlers  */
	FTimerHandle BrakeHandle; //Handbrake Handle
	FTimerHandle JumpHandle; // Jump Handle

	float mMovementSpeed;
	float mMaxVelocity;

	float mEngineSpeed;
	float mEngineAcceleration;
	
	float mRotateRate;
	float mRotationSpeed;

	float mTowerRotX;
	float mTowerRotY;
	
	FName boneName;
	FName WeaponSocket;

	FVector *impulseForce;



#pragma endregion

public:
	// Sets default values for this pawn's properties
	ARover();

#pragma region Movement

	/** Handle pressing forwards */
	void MoveForward(float value);
	/** Handle pressing right */
	void MoveRight(float value);

	/** Handle Movement Vector */
	FVector GetMovementVector();

	/** Add Movement */
	void AddMovement(ARover *self, FVector forceVector);

	/** Add Engine Speed for Animations and Sound */
	void AddEngineSpeed(ARover *self, float speed, float mul);

	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();

	/** Handle Jump with vehicle */
	void OnJump();
	/** Handle Jump released*/
	void OnJumpRelease();

#pragma endregion
	
	/*
	UMaterialInstanceDynamic* DynMaterial;
	UMaterialInstanceDynamic* MainMaterial;
	FLinearColor RoverColor;
	*/
	void UpdateEngineSound();

	void UpdateSkControlsAndMats();

	void RepaintRover(ARover *self);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	/** Returns EngineSoundComponent subobject **/
	FORCEINLINE UAudioComponent* GetEngineSound() const { return EngineSound; }

};
