// Fill out your copyright notice in the Description page of Project Settings.

#include "Rover.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "Classes/Materials/Material.h"
#include "Classes/Animation/AnimInstance.h"
#include "OutputDevice.h"
#include "OutputDeviceNull.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "SpeechBubble.h"

// Sets default values
ARover::ARover()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	
	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mesh_body(TEXT("SkeletalMesh'/Game/MAARS/Meshes/Skeletal/SM_Maars_SK.SM_Maars_SK'"));
	Mesh->SetSkeletalMesh(Mesh_body.Object);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh->SetAngularDamping(0.02);
	Mesh->bGenerateOverlapEvents = 1;
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	// Animation Blueprint
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/MAARS/Blueprints/AnimBP_MAARS"));
	Mesh->SetAnimInstanceClass(AnimBPClass.Class);

	/** Additinal Meshes */
	WeaponSocket = FName(TEXT("S_WEAPON"));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Weapon_mesh(TEXT("SkeletalMesh'/Game/MAARS/Meshes/Skeletal/SM_M240B_SK.SM_M240B_SK'"));
	Weapon->SetSkeletalMesh(Weapon_mesh.Object);
	Weapon->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform,WeaponSocket);
	Weapon->SetVisibility(false);

	RoverGlass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Glass_mesh(TEXT("/Game/MAARS/Meshes/Static/SM_Maars_Glass"));
	RoverGlass->SetStaticMesh(Glass_mesh.Object);
	RoverGlass->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
	RoverGlass->SetRelativeLocation(FVector(1.789685, -3.691504, -10.276528));

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeRotation(FRotator(-20.f, -180.0f, 0.f));
	SpringArm->SetupAttachment(Weapon);
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 10.0f;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create audio component
	EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSound->SetupAttachment(Weapon);
	static ConstructorHelpers::FObjectFinder<USoundCue> EngineCue(TEXT("/Game/MAARS/Sounds/S_Engine-loop_Cue"));
	EngineSound->SetSound(EngineCue.Object);
	
	
	/** Materials */ 
	RoverColor = FLinearColor(FColor::White);
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat0(TEXT("MaterialInstanceConstant'/Game/MAARS/Materials/M_MAARS_1_Inst.M_MAARS_1_Inst'"));
	if (Mat0.Succeeded())
	{
		RoverMaterial = (UMaterial*)Mat0.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Mat1(TEXT("MaterialInstanceConstant'/Game/MAARS/Materials/M_MAARS_2_Inst.M_MAARS_2_Inst'"));
	if (Mat1.Succeeded())
	{
		WheelsMaterial = (UMaterial*)Mat1.Object;
	}

	if (!UKismetSystemLibrary::IsValid(MainMaterial))
	{
		MainMaterial = UMaterialInstanceDynamic::Create(RoverMaterial, Mesh);
		if (MainMaterial)
		{
			Mesh->SetMaterial(0, MainMaterial);
		}
		
		

		if (!UKismetSystemLibrary::IsValid(DynMaterial))
		{
			DynMaterial = UMaterialInstanceDynamic::Create(WheelsMaterial, Mesh);
			if (DynMaterial)
			{
				Mesh->SetMaterial(1, DynMaterial);
				RepaintRover(this);
			}
		}
	}

	SpeechSocket = FName(TEXT("SpeechSocket"));
	
	/* Creation of SpeechBubble Class Reference */
	static ConstructorHelpers::FObjectFinder<UClass> SpeechBubbleClass_obj(TEXT("Class'/Script/Masihini.SpeechBubble'"));
	if (SpeechBubbleClass_obj.Object)
	{
		SpeechBubbleClass = SpeechBubbleClass_obj.Object;
	}

	
	
	bIsJumping = false;
	bIsRotating = false;
	bIsStopped = false;
	bIsSpeaking = false;

	mMovementSpeed = 100000.0f;
	mMaxVelocity = 500.0f;
	metersMoved = 0;

	mEngineSpeed = 0.0f;
	mEngineAcceleration = 0.0f;
	
	mRotateRate = 0.0f;
	mRotationSpeed = 5000.0f;

	mTowerRotX = 0.0f;
	mTowerRotY = 0.0f;

	boneName = FName(TEXT("None"));
	impulseForce = new FVector(0.0f, 0.0f, 1000.0f);
	
	
	
	currentInstruction = 0;
	
	
}


// Called when the game starts or when spawned
void ARover::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARover::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSkControlsAndMats();

	UpdateEngineSound();
	
	UpdateSpeakingText();
	
}

// Called to bind functionality to input
void ARover::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	//PlayerInputComponent->BindAxis("MoveForward", this, &ARover::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &ARover::MoveRight);

	//PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ARover::OnHandbrakePressed);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARover::OnJump);
}

void ARover::MoveForward(float value, int mts)
{
	
	if (!bIsStopped && mRotateRate == 0.0f)
	{
		FVector movementVector = GetMovementVector();
		FVector forceVector = movementVector * (value * mMovementSpeed);
		AddMovement(this, forceVector);
		AddEngineSpeed(this, value, -0.05f);
		
		if (mts == 1)
		{
			isInstructionDone = true;
			mEngineAcceleration = 0.0f;
			GetWorldTimerManager().SetTimer(InstructionHandle, this, &ARover::StartMasihiniExecution, 1.0f, false);
		}
		else {
			//Binding the function with specific values
			TimerDelegate.BindUFunction(this, FName("MoveForward"), value, mts-1);
			//Calling MoveForward after one second
			GetWorldTimerManager().SetTimer(MovementHandle, TimerDelegate, 0.25f, false);
		}
	}

}

void ARover::MoveRight(float value)
{
	
	mRotateRate = value;
	if (!bIsStopped)
	{
		float vectorZ = UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * mRotationSpeed * value * 1.4f;
		FVector angularVelocity = UKismetMathLibrary::MakeVector(0.0f, 0.0f, vectorZ);
		Mesh->SetPhysicsAngularVelocityInDegrees(angularVelocity, false, boneName);
		AddEngineSpeed(this, value, -0.03f);
		mRotateRate = 0.0f;
		mEngineAcceleration = 0.0f;
		GetWorldTimerManager().SetTimer(InstructionHandle, this, &ARover::StartMasihiniExecution, 0.5f, false);
	}

}

FVector ARover::GetMovementVector()
{
	
	FVector startLocation = GetActorLocation();
	FVector endLocation = startLocation;
	endLocation.Z = startLocation.Z + 300.0f;
	
	FVector halfSize = FVector(60.0f, 60.0f, 1.0f);
	
	FRotator rotator = FRotator(0.0f, 0.0f, 0.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult Hit;
	
	UKismetSystemLibrary::BoxTraceSingleForObjects(
		GetWorld(), 
		startLocation, 
		endLocation, 
		halfSize, 
		rotator, 
		ObjectTypes, 
		false, 
		ActorsToIgnore, 
		EDrawDebugTrace::None, 
		Hit, 
		true
	);

	FVector NormalHit = Hit.Normal;

	FRotator worldRotation = Mesh->GetComponentRotation();
	FVector rightVector = UKismetMathLibrary::GetRightVector(worldRotation) * -1.0f;
	
	return UKismetMathLibrary::Cross_VectorVector(NormalHit, rightVector);
}

void ARover::AddMovement(ARover *self, FVector forceVector)
{
	if (UKismetMathLibrary::VSize(GetVelocity()) <= mMaxVelocity)
	{
		Mesh->AddForce(forceVector, boneName, false);
	}
}

void ARover::AddEngineSpeed(ARover *self, float speed, float mul)
{
	mEngineSpeed += (speed * mul);
	float temp = ( ( UKismetMathLibrary::Abs(speed) * 0.4f ) + mEngineAcceleration ) - 0.1; 
	mEngineAcceleration  = FMath::Clamp(temp, 0.0f, 12.0f);
}

void ARover::UpdateEngineSound()
{

	if (mEngineAcceleration == 0.0f)
	{
		EngineSound->Stop();
	}
	else
	{

		if (!EngineSound->IsPlaying())
		{ 
			EngineSound->Play(0.0f);
		}

		float soundMultiplier = mEngineAcceleration / 12.0f;
		soundMultiplier = FMath::Clamp(soundMultiplier, 0.1f, 1.0f);
		EngineSound->SetPitchMultiplier(soundMultiplier);
		EngineSound->SetVolumeMultiplier(soundMultiplier);

	}

}

void ARover::UpdateSkControlsAndMats()
{
	if (mEngineAcceleration != 0.0f)
	{
		AddEngineSpeed(this, 20.0f, -0.05f);
	}
	DynMaterial->SetScalarParameterValue("TilingSpeed", mEngineSpeed*(-0.1f));
	
	UAnimInstance* animBP = Cast<UAnimInstance>(Mesh->GetAnimInstance());
	if (!animBP) { return; }

	FOutputDeviceNull ar;
	const FString speedCommand = FString::Printf(TEXT("SetSpeed %f"), mEngineSpeed);
	animBP->CallFunctionByNameWithArguments(*speedCommand, ar, NULL, true);

	const FString towerXCommand = FString::Printf(TEXT("SetTowerRotX %f"), mTowerRotX);
	animBP->CallFunctionByNameWithArguments(*towerXCommand, ar, NULL, true);
	
	const FString towerYCommand = FString::Printf(TEXT("SetTowerRotY %f"), mTowerRotY);
	animBP->CallFunctionByNameWithArguments(*towerYCommand, ar, NULL, true);

}

void ARover::RepaintRover(ARover *self)
{
	
	MainMaterial->SetVectorParameterValue("Color", RoverColor);
	
}

void ARover::OnHandbrakePressed()
{
	bIsStopped = true;
	Mesh->SetPhysicsLinearVelocity(FVector(0.0f,0.0f,0.0f), false, boneName);

	mEngineAcceleration = 0.0f;
	GetWorldTimerManager().SetTimer(BrakeHandle, this, &ARover::OnHandbrakeReleased, 0.2f, false);
}

void ARover::OnHandbrakeReleased()
{
	bIsStopped = false; 
	GetWorldTimerManager().SetTimer(InstructionHandle, this, &ARover::StartMasihiniExecution, 0.5f, false);
}

void ARover::OnJump()
{
	if (this->GetVelocity().Z >= 0.0f && !bIsJumping)
	{
		
		if (Mesh->GetBodyInstance())
		{
			FBodyInstance *bodyInstance = Mesh->GetBodyInstance();
			bodyInstance->bLockXRotation = true;
			bodyInstance->bLockYRotation = true;
			bodyInstance->bLockZRotation = true;
			bodyInstance->SetDOFLock(EDOFMode::SixDOF);
		}
		Mesh->SetPhysicsLinearVelocity(*impulseForce, true, boneName);
		bIsJumping = true;
		GetWorldTimerManager().SetTimer(JumpHandle, this, &ARover::OnJumpRelease, 1.0f, false);
	}
}

void ARover::OnJumpRelease()
{

	if (Mesh->GetBodyInstance())
	{
		FBodyInstance *bodyInstance = Mesh->GetBodyInstance();
		bodyInstance->bLockXRotation = false;
		bodyInstance->bLockYRotation = false;
		bodyInstance->bLockZRotation = false;
		bodyInstance->SetDOFLock(EDOFMode::Default);
	}

	bIsJumping = false;

	GetWorldTimerManager().SetTimer(InstructionHandle, this, &ARover::StartMasihiniExecution, 0.5f, false);

}

void ARover::OnSpeak(FString dialogue)
{
	SocketLoc = Mesh->GetSocketLocation(SpeechSocket);
	SocketLoc += FVector(200.0f, 0.0f, 0.0f);
	SocketRot = FRotator(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	if (!Speech)
	{
		Speech = GetWorld()->SpawnActor<ASpeechBubble>(SpeechBubbleClass, SocketLoc, SocketRot, SpawnParams);
		Speech->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform, SpeechSocket);
	}
	else{
		Speech->GetMesh()->SetVisibility(true, false);
		Speech->GetTextRender()->SetVisibility(true, false);
	}

	FText dialogueText = FText::FromString(dialogue);
	Speech->GetTextRender()->SetText(dialogueText);
	GetWorldTimerManager().SetTimer(InstructionHandle, this, &ARover::StartMasihiniExecution, 2.0f, false);

}	

void ARover::UpdateSpeakingText()
{
	
}

void ARover::SetInstructionsSize(int newSize)
{
	instructionsSize = newSize;
}

void ARover::SetInstructions(TArray<FString> newInstructions)
{
	instructions = newInstructions;
}

void ARover::AnalyzeInstruction(FString instruction)
{
	if (instruction.Contains("ACCEL"))
	{
		int32 meters = FCString::Atoi(*instruction.RightChop(5));
		UE_LOG(LogTemp, Warning, TEXT("Move %d"), meters);
		int direction = (-1)*(meters / meters);
		meters = FMath::Abs(meters);
		MoveForward(direction*20.0f, meters);
	}
	else if(instruction.Contains("ROT"))
	{
		int32 angles = FCString::Atoi(*instruction.RightChop(3));
		UE_LOG(LogTemp, Warning, TEXT("Rotate %d"), angles);
		float anglesRover = 0.0f;
		
		/** UE4 angles goes from -180 to 180 */
		anglesRover = angles / 2.0f;
		anglesRover = anglesRover / 10.0f;
		anglesRover = FMath::Clamp(anglesRover, -180.0f, 180.0f);
		MoveRight(anglesRover);
	}
	else if (instruction.Contains("STOP"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop"));
		OnHandbrakePressed();
	}
	else if (instruction.Contains("JUMP"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Jump"));
		OnJump();
	}
	else if (instruction.Contains("SPEAK"))
	{
		instruction = instruction.RightChop(5);
		UE_LOG(LogTemp, Warning, TEXT("Speak: %s"), *instruction);
		bIsSpeaking = true;
		OnSpeak(instruction);
	}
	else{
		GetWorldTimerManager().SetTimer(InstructionHandle, this, &ARover::StartMasihiniExecution, 0.5f, false);
	}

}

void ARover::StartMasihiniExecution() 
{
	if (Speech)
	{
		Speech->GetMesh()->SetVisibility(false, false);
		Speech->GetTextRender()->SetVisibility(false, false);
	}

	if (currentInstruction < instructionsSize)
	{
		FString instruction = instructions[currentInstruction];
		AnalyzeInstruction(instructions[currentInstruction]);
		currentInstruction++;
	}
	
}


