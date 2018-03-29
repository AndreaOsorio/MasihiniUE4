// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteFile.generated.h"

/**
 * 
 */
UCLASS()
class MASIHINI_API UReadWriteFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

		UFUNCTION(BlueprintPure, Category = "Masihini", meta = (Keywords = "LoadCode"))
			static bool FileLoadCode(FString FileNameA, FString& SaveTextA);

		UFUNCTION(BlueprintPure, Category = "Masihini", meta = (Keywords = "SaveCode"))
			static bool FileSaveCode(FString SaveTextB, FString FileNameB);
	
	
	
	
};
