// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "CoreMinimal.h"
#include "Rover.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAudioComponent;
class UTextRenderComponent;

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

	//Speech Bubble class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Masihini")
		TSubclassOf<class ASpeechBubble> SpeechBubbleClass;

	ASpeechBubble* Speech;

	

#pragma endregion

#pragma region Variables

	/** Are we in the air */
	bool bIsJumping;

	/** Are we rotating */
	bool bIsRotating;

	/** Are we stopped */
	bool bIsStopped;

	/** Is Rover speaking? */
	bool bIsSpeaking;

	/** Time Handlers  */
	FTimerHandle BrakeHandle; //Handbrake Handle
	FTimerHandle JumpHandle; // Jump Handle
	FTimerHandle MovementHandle; // Movement Handle

	FTimerDelegate TimerDelegate;  //  Used for binding UFunctions with parameters using Timer Handlers

	float mMovementSpeed;
	float mMaxVelocity;

	int metersMoved;

	float mEngineSpeed;
	float mEngineAcceleration;
	
	float mRotateRate;
	float mRotationSpeed;

	float mTowerRotX;
	float mTowerRotY;

	float TextOpacity;
	FName opacityScalar;

	FName boneName;
	FName WeaponSocket;
	FName SpeechSocket;
	FVector SocketLoc;
	FRotator SocketRot;

	FVector *impulseForce;





#pragma endregion

#pragma region Masihini Variables

	int instructionsSize;
	TArray<FString> instructions;

	int currentInstruction;

	FTimerHandle InstructionHandle; // Masihini Handle

	bool isInstructionDone;

#pragma endregion

public:
	// Sets default values for this pawn's properties
	ARover();

#pragma region Mechanics

	/** Handle pressing forwards */
	UFUNCTION()
		void MoveForward(float value, int mts);
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
	/** Handle Jump released */
	void OnJumpRelease();

	/** Handle Rover Speak */
	void OnSpeak(FString dialogue);
	void UpdateSpeakingText();


#pragma endregion
	
#pragma region Materials & Sounds

	UMaterialInstanceDynamic* MainMaterial; // Main Material (For Color)
	UMaterialInstanceDynamic* DynMaterial;  // Dynamic Material (For Wheels) 

	UMaterialInstanceDynamic* TextMaterialInst;  // Text Material (For Text Render Component) 


	FLinearColor RoverColor;

	UMaterial* RoverMaterial;
	UMaterial* WheelsMaterial;
	UMaterial* TextMaterial;
	
	void UpdateSkControlsAndMats();
	void RepaintRover(ARover *self);
	void UpdateEngineSound();

#pragma endregion

#pragma region Masihini Functions

	void SetInstructionsSize(int newSize);

	void SetInstructions(TArray<FString> newInstructions);

	void AnalyzeInstruction(FString instruction);

	void StartMasihiniExecution();

	

#pragma endregion

#pragma region Game Conditions

	void OnDie();

	void OnWin();

#pragma endregion


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
