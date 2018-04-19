// Fill out your copyright notice in the Description page of Project Settings.

#include "SpeechBubble.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/Materials/Material.h"
#include "Classes/Materials/MaterialInstance.h"

// Sets default values
ASpeechBubble::ASpeechBubble()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/**Initialization of Components */
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	// Static Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh_body(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	if (Mesh_body.Succeeded())
	{
		Mesh->SetStaticMesh(Mesh_body.Object);
	}

	// Mesh Material
	static ConstructorHelpers::FObjectFinder<UMaterial> Mesh_Mat(TEXT("Material'/Game/UI/Materials/TXT_SpeechBubble_Mat.TXT_SpeechBubble_Mat'"));
	if (Mesh_Mat.Succeeded())
	{
		Material = (UMaterialInstance*) Mesh_Mat.Object;
		Mesh->SetMaterial(0, Material);
	}
	
	Mesh->SetSimulatePhysics(false);
	Mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	Mesh->SetRelativeScale3D(FVector(5.0f, 2.0f, 1.5f));
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Text Render Component
	MasihiniText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	MasihiniText->SetTextRenderColor(FColor::Black);
	MasihiniText->SetupAttachment(RootComponent);
	MasihiniText->SetRelativeRotation(FRotator(90.0f, 0.0f, -90.0f));
	MasihiniText->SetRelativeLocation(FVector(0.0f, 0.0f, 1.0f));
	MasihiniText->WorldSize = 18.0f;
	MasihiniText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	MasihiniText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextBottom);
	
}

// Called when the game starts or when spawned
void ASpeechBubble::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ASpeechBubble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

