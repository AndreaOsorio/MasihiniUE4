// Fill out your copyright notice in the Description page of Project Settings.

#include "Goal.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerBox.h"


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

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mesh_mat(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Gold.M_Metal_Gold'"));
	Mesh->SetMaterial(0, Mesh_mat.Object);
	

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

