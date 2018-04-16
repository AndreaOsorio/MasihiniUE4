// Fill out your copyright notice in the Description page of Project Settings.

#include "ReadWriteFile.h"
#include "FileHelper.h"
#include "Paths.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Rover.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"

using namespace std;

bool UReadWriteFile::FileLoadCode(FString FileNameA, FString& SaveTextA)
{
	return FFileHelper::LoadFileToString(SaveTextA, *(FPaths::ProjectDir() + FileNameA));
}

bool UReadWriteFile::FileSaveCode(FString SaveTextB, FString FileNameB)
{
	return FFileHelper::SaveStringToFile(SaveTextB, *(FPaths::ProjectDir() + FileNameB));
}

void UReadWriteFile::TransferMasihini()
{
	FString pathCopy = FPaths::ProjectDir() + "copy.bat";
	const char* commandCopy = TCHAR_TO_ANSI(*pathCopy);
	if (system(NULL)) puts("Ok");
	else exit(EXIT_FAILURE);
	int i = system(commandCopy);
}

void UReadWriteFile::CompileMasihini()
{
	//FPlatformProcess::CreateProc(TEXT("E:\\Documents\\ITESM\\CompilerDesign\\Masihini\\run.exe"), nullptr, true, false, false, nullptr, 0, nullptr, nullptr);
	
	FString pathRun = FPaths::ProjectDir() + "test.bat";
	const char* commandRun = TCHAR_TO_ANSI(*pathRun);
	int i = system(commandRun);
	
}

void UReadWriteFile::ExecuteMasihini(UObject * WorldContextObject)
{
	FString pathExec = FPaths::ProjectDir() + "log.txt";
	TArray<FString> lines;
	FString FileData;
	bool valid = FFileHelper::LoadFileToString(FileData, *pathExec);
	int32 lineCount = FileData.ParseIntoArray(lines, LINE_TERMINATOR, true);
	ARover *player;

	if (GEngine && GEngine->GameViewport)
	{
		UWorld * World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		if (World)
		{
			for (TActorIterator<ARover> ActorItr(World); ActorItr; ++ActorItr)
			{
				player = Cast<ARover>(*ActorItr);
				if (player)
				{
					player->SetInstructionsSize(lineCount);
					player->SetInstructions(lines);
					player->StartMasihiniExecution();
				}
				break;
			}
		}
	}
	

}

