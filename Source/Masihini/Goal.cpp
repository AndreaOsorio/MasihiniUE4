// Fill out your copyright notice in the Description page of Project Settings.

#include "Goal.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerBox.h"
#include "Rover.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EngineUtils.h"


// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh_body(TEXT("StaticMesh'/Game/StarterContent/Architecture/SM_AssetPlatform.SM_AssetPlatform'"));
	Mesh->SetStaticMesh(Mesh_body.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mesh_mat(TEXT("Material'/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse.M_Tech_Hex_Tile_Pulse'"));
	Mesh->SetMaterial(0, Mesh_mat.Object);
	

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(50.0f,50.0f,50.0f));
	TriggerBox->SetRelativeLocation(FVector(0.0f,0.0f,60.0f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	TriggerBox->bGenerateOverlapEvents = true;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlap);

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGoal::CheckPlayer()
{
	if (GEngine && GEngine->GameViewport)
	{
		if (GEngine->GetWorld())
		{
			for (TActorIterator<ARover> ActorItr(GEngine->GetWorld()); ActorItr; ++ActorItr)
			{
				//Player = Cast<ARover>(*ActorItr);
				break;
			}
		}
	}
	return false;
}

void AGoal::OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		FName nextLevel;
		currentLevel = UGameplayStatics::GetCurrentLevelName(this->GetWorld(), true);
		int32 levelNumber = FCString::Atoi(*currentLevel.RightChop(5));

		switch (levelNumber)
		{
		case 1:
			nextLevel = FName(TEXT("Level2"));
			break;

		case 2:
			nextLevel = FName(TEXT("Level3"));
			
			break;

		case 3:
			nextLevel = FName(TEXT("Level1"));
			break;
		
		}
		UGameplayStatics::OpenLevel(GetWorld(), nextLevel, true, "");
		
	}

}



 