// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeechBubble.generated.h"


class UTextRenderComponent;

UCLASS()
class MASIHINI_API ASpeechBubble : public AActor
{
	GENERATED_BODY()

private:
	/** Mesh Component that will be the Root Component */
	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* Mesh;

	/** Text Component that will show the speak from Masihini */
	UPROPERTY(EditDefaultsOnly)
		UTextRenderComponent* MasihiniText;


	UPROPERTY(EditDefaultsOnly)
		UMaterialInstance* Material;




public:	
	// Sets default values for this actor's properties
	ASpeechBubble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns TextRenderComponent subobject **/
	FORCEINLINE UTextRenderComponent* GetTextRender() const { return MasihiniText; }

	/** Returns StaticMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	
	
};
