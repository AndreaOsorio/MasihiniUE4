// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"


class UBoxComponent;

UCLASS()
class MASIHINI_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* TriggerBox;

	//ARover* Player;
	

private:

	bool CheckPlayer();

	FString currentLevel;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
			void OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);



	
	
};
